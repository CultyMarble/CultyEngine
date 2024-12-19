#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Windows.h"
#include "GameState.h"
#include "ComponentNote.h"
#include "ComponentFeedbackPopup.h"

extern "C"
{
#include <FFmpeg/include/libavformat/avformat.h>
#include <FFmpeg/include/libavcodec/avcodec.h>
#include <FFmpeg/include/libswscale/swscale.h>
#include <FFmpeg/include/libavutil/imgutils.h>
}

namespace fs = std::filesystem;

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Audio;

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "ComponentNoteMovement")
        return gameObject.AddComponent<ComponentNote>();
    if (componentName == "ComponentFeedbackPopup")
        return gameObject.AddComponent<ComponentFeedbackPopup>();
    return nullptr;
}

Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "ComponentNoteMovement")
        return gameObject.GetComponent<ComponentNote>();
    if (componentName == "ComponentFeedbackPopup")
        return gameObject.GetComponent<ComponentFeedbackPopup>();
    return nullptr;
}

void GameState::Initialize()
{
    // Load sound effects
    auto* soundManager = CultyEngine::Audio::SoundEffectManager::Get();
    mSoundIdHit_Cool = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/hit_empty.wav");
    mSoundIdHit_Empty = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/hit_cool.wav");
    mSoundIdHit_Regular = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/hit_regular.wav");
    mSoundIdMiss = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/miss.wav");

    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    LoadTimeline(L"../../Assets/Templates/ProjectDiva/gameplay_timeline.json");

    // CleanFrameFolder("../../Assets/Videos/ppmFrames");
    // ExtractFramesFromVideo("../../Assets/Videos/rnc.mp4", "../../Assets/Videos/ppmFrames");
    LoadPPMFramesAsTextures("../../Assets/Videos/ppmFrame");

    mGameWorld.LoadLevel(L"../../Assets/Templates/ProjectDiva/gameplay_level.json");

    CreateFullscreenUISpriteWithTexture();

    mBackgroundMusic = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/rnc.wav");
    Audio::SoundEffectManager::Get()->Play(mBackgroundMusic, true);
}

void GameState::Terminate()
{
    for (auto* texture : mTextures)
    {
        if (texture != nullptr)
        {
            texture->Terminate();
            delete texture;
        }
    }
    mTextures.clear();

    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mElapsedTime += deltaTime;

    static float frameTimer = 0.0f;
    const float frameDuration = 1.0f / 59.0f;

    frameTimer += deltaTime;
    while (frameTimer >= frameDuration)
    {
        frameTimer -= frameDuration; // Reset timer
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mTextures.size();

        // Update the fullscreen UI sprite texture
        UpdateFullscreenUISpriteTexture();
    }

    SpawnNote();

    // Update the game world
    mGameWorld.Update(deltaTime);

    // Update the audio system
    CultyEngine::Audio::AudioSystem::Get()->Update();

    // Check and update the tracked note
    if (mNoteQueue.empty() == false && mGameWorld.GetGameObject(mTrackedNote) == nullptr)
    {
        mTrackedNote = mNoteQueue.front();
        mNoteQueue.pop();
    }

    HandleTrackedNote();

    HandlePlayerInput();
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mGameWorld.DebugUI();
        ImGui::Text("Elapsed Time: %.2f", mElapsedTime);
        ImGui::Text("Current Frame: %zu", mCurrentFrameIndex);
    ImGui::End();
}

void GameState::LoadTimeline(const std::filesystem::path& timelineFile)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, timelineFile.u8string().c_str(), "r");
    ASSERT(err == 0 && file != nullptr, "GameState: Failed to load timeline file");

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);
    ASSERT(doc.IsObject() && doc.HasMember("Timeline"), "GameState: Invalid timeline format");

    const auto& notes = doc["Timeline"].GetArray();
    for (const auto& note : notes)
    {
        // Read time to spawn
        float time = note["Time"].GetFloat();

        // Read note template
        std::string templatePath = note["Template"].GetString();

        // Read override components
        std::string componentsJson = "";
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

        if (note.HasMember("Components"))
        {
            writer.StartObject();                    // Start the outer object
            writer.Key("Components");                // Add the "Components" key
            note["Components"].Accept(writer);       // Serialize the "Components" value
            writer.EndObject();                      // End the outer object
            componentsJson = buffer.GetString();     // Final wrapped JSON string
        }

        mNotes.emplace_back(time, templatePath, componentsJson);
    }
}

void GameState::SpawnNote()
{
    while (!mNotes.empty() && mElapsedTime >= std::get<0>(mNotes.front()))
    {
        const auto& [time, templatePath, componentsJson] = mNotes.front();

        // Create the note
        GameObject* noteObject = mGameWorld.CreateGameObject("Note", templatePath);

        // Parse the components JSON string into a document
        if (componentsJson.empty() == false)
        {
            rapidjson::Document componentsDoc;
            componentsDoc.Parse(componentsJson.c_str());
            GameObjectFactory::OverrideDeserialize(componentsDoc, *noteObject);
        }

        noteObject->Initialize();

        // Get the movement component and set the destruction callback
        auto* componentNode = noteObject->GetComponent<ComponentNote>();

        componentNode->SetOnDestroyedCallback([this](CultyEngine::GameObjectHandle handle, CultyEngine::MathC::Vector2 position)
            {
                if (handle.GetIndex() == mTrackedNote.GetIndex())
                {
                    SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_sad.json", position, 0.25f);
                    LOG("Miss: Note disappeared.");
                    mTrackedNote = CultyEngine::GameObjectHandle();
                }
            });

        // Store the note handle in the queue
        mNoteQueue.push(noteObject->GetHandle());

        // Remove the note from the timeline
        mNotes.erase(mNotes.begin());
    }
}

void GameState::HandleTrackedNote()
{
    GameObject* trackedObject = mGameWorld.GetGameObject(mTrackedNote);
    if (trackedObject == nullptr)
        return;

    auto* note = trackedObject->GetComponent<ComponentNote>();
    if (note == nullptr)
        return;

    // Activate the note if it's within the active zone
    if (!note->IsActive() && note->TimeRemaining() <= note->GetTotalTime() * 0.4f)
        note->SetActive(true);
}

void GameState::HandlePlayerInput()
{
    auto* inputSystem = Input::InputSystem::Get();
    auto* soundManager = CultyEngine::Audio::SoundEffectManager::Get();

    bool isAnyButtonPressed = false;

    // Check if any valid button is pressed
    for (int button : mValidButtons)
    {
        if (inputSystem->IsControllerButtonDown(button))
        {
            isAnyButtonPressed = true;
            break;
        }
    }

    // Play empty hit sound if no tracked note exists or note is inactive
    GameObject* trackedObject = mGameWorld.GetGameObject(mTrackedNote);
    if (!trackedObject || !trackedObject->GetComponent<ComponentNote>()->IsActive())
    {
        if (isAnyButtonPressed)
            soundManager->Play(mSoundIdHit_Empty);
        return;
    }

    auto* note = trackedObject->GetComponent<ComponentNote>();

    // Process player input for the tracked note
    for (int button : mValidButtons)
    {
        if (!inputSystem->IsControllerButtonDown(button))
            continue;

        float timeRemaining = note->TimeRemaining();
        float totalTime = note->GetTotalTime();

        // Process hits or misses within the active timing window
        if (timeRemaining <= totalTime * 0.1f)
        {
            if (note->IsCorrectButtonDown(button) == false)
            {
                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_sad.json", position, 0.25f);

                LOG("Miss: Incorrect button.");
                soundManager->Play(mSoundIdMiss);

                note->Terminate();
                mTrackedNote = CultyEngine::GameObjectHandle();
                return;
            }

            // Determine hit quality and play corresponding sound
            if (timeRemaining <= totalTime * 0.02f)
            {
                LOG("Hit: Cool (Golden)");
                soundManager->Play(mSoundIdHit_Cool);

                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_cool1.json", position, 0.25f);
            }
            else if (timeRemaining <= totalTime * 0.03f)
            {
                LOG("Hit: Cool (Normal)");
                soundManager->Play(mSoundIdHit_Regular);

                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_cool2.json", position, 0.25f);
            }
            else if (timeRemaining <= totalTime * 0.04f)
            {
                LOG("Hit: Fine");
                soundManager->Play(mSoundIdHit_Regular);

                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_fine.json", position, 0.25f);
            }
            else if (timeRemaining <= totalTime * 0.05f)
            {
                LOG("Hit: Safe");
                soundManager->Play(mSoundIdHit_Regular);

                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_safe.json", position, 0.25f);
            }
            else if (timeRemaining <= totalTime * 0.07f)
            {
                LOG("Hit: Worst");
                soundManager->Play(mSoundIdHit_Regular);

                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_worst.json", position, 0.25f);
            }
            else
            {
                LOG("Hit: Miss");
                soundManager->Play(mSoundIdMiss);

                auto position = note->GetCurrentPosition();
                SpawnFeedbackPopup("../../Assets/Templates/ProjectDiva/popup_sad.json", position, 0.25f);
            }

            // Terminate the note after processing a hit
            note->Terminate();
            mTrackedNote = CultyEngine::GameObjectHandle();
            return;
        }
    }

    // Play empty hit sound for valid button presses outside active timing window
    if (isAnyButtonPressed)
        soundManager->Play(mSoundIdHit_Empty);
}

void GameState::SpawnFeedbackPopup(const std::string& templatePath, const MathC::Vector2& position, float lifetime)
{
    // Create the popup GameObject
    GameObject* popupObject = mGameWorld.CreateGameObject("FeedbackPopup", templatePath);
    ASSERT(popupObject != nullptr, "Failed to create FeedbackPopup!");

    // Set the position
    popupObject->GetComponent<ComponentUISprite>()->SetPosition({ position.x, position.y - 50});

    // Set the lifetime in the FeedbackPopup component
    auto* feedbackPopup = popupObject->GetComponent<ComponentFeedbackPopup>();
    if (feedbackPopup != nullptr)
        feedbackPopup->SetLifetime(lifetime);

    // Initialize the GameObject
    popupObject->Initialize();
}

// VIDEO PLAYER

void GameState::ExtractFramesFromVideo(const std::string& videoPath, const std::string& outputFolder)
{
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    const AVCodec* codec = nullptr;
    AVFrame* frame = nullptr;
    AVFrame* rgbFrame = nullptr;
    AVPacket* packet = nullptr;
    SwsContext* swsContext = nullptr;

    int videoStreamIndex = -1;

    // Open the video file
    ASSERT(avformat_open_input(&formatContext, videoPath.c_str(), nullptr, nullptr) == 0,
        "Failed to open video file!");

    ASSERT(avformat_find_stream_info(formatContext, nullptr) >= 0,
        "Failed to retrieve stream info!");

    // Find the video stream
    for (unsigned int i = 0; i < formatContext->nb_streams; ++i)
    {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
            break;
        }
    }
    ASSERT(videoStreamIndex != -1, "No video stream found in file!");

    // Set up codec context
    AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    codec = avcodec_find_decoder(codecParams->codec_id);
    ASSERT(codec != nullptr, "Failed to find codec!");

    codecContext = avcodec_alloc_context3(codec);
    ASSERT(codecContext != nullptr, "Failed to allocate codec context!");

    ASSERT(avcodec_parameters_to_context(codecContext, codecParams) >= 0,
        "Failed to initialize codec context!");

    ASSERT(avcodec_open2(codecContext, codec, nullptr) >= 0, "Failed to open codec!");

    // Allocate structures
    frame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    packet = av_packet_alloc();
    ASSERT(frame != nullptr && rgbFrame != nullptr && packet != nullptr, "Failed to allocate FFmpeg resources!");

    swsContext = sws_getContext(
        codecContext->width, codecContext->height, codecContext->pix_fmt,
        codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, nullptr, nullptr, nullptr);
    ASSERT(swsContext != nullptr, "Failed to create SwsContext!");

    // Allocate buffer for RGB frame
    int rgbBufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
    uint8_t* rgbBuffer = (uint8_t*)av_malloc(rgbBufferSize);
    ASSERT(rgbBuffer != nullptr, "Failed to allocate RGB buffer!");

    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbBuffer, AV_PIX_FMT_RGB24,
        codecContext->width, codecContext->height, 1);

    // Frame extraction loop
    int frameIndex = 0;
    while (av_read_frame(formatContext, packet) >= 0)
    {
        if (packet->stream_index == videoStreamIndex)
        {
            avcodec_send_packet(codecContext, packet);
            while (avcodec_receive_frame(codecContext, frame) == 0)
            {
                // Convert to RGB24
                sws_scale(swsContext, frame->data, frame->linesize, 0,
                    codecContext->height, rgbFrame->data, rgbFrame->linesize);

                // Save frame as PPM
                std::string filename = outputFolder + "/frame_" + std::to_string(frameIndex++) + ".ppm";
                FILE* file = nullptr;
                fopen_s(&file, filename.c_str(), "wb");
                ASSERT(file != nullptr, "Failed to open file for writing!");

                fprintf(file, "P6\n%d %d\n255\n", codecContext->width, codecContext->height);
                for (int y = 0; y < codecContext->height; ++y)
                {
                    fwrite(rgbFrame->data[0] + y * rgbFrame->linesize[0], 1, codecContext->width * 3, file);
                }
                fclose(file);
            }
        }
        av_packet_unref(packet);
    }

    // Cleanup
    av_free(rgbBuffer);
    av_frame_free(&rgbFrame);
    av_packet_free(&packet);
    av_frame_free(&frame);
    sws_freeContext(swsContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

void GameState::CleanFrameFolder(const std::string& outputFolder)
{
    if (fs::exists(outputFolder))
    {
        for (const auto& entry : fs::directory_iterator(outputFolder))
        {
            if (entry.path().extension() == ".ppm")
                fs::remove(entry);
        }
    }
    else
    {
        fs::create_directories(outputFolder);
    }
}

void GameState::LoadPPMFramesAsTextures(const std::string& folderPath)
{
    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.path().extension() == ".ppm")
        {
            // Load PPM file
            std::ifstream file(entry.path(), std::ios::binary);
            ASSERT(file.is_open(), "Failed to open PPM file!");

            std::string header;
            int width, height, maxVal;

            // Read PPM header
            file >> header >> width >> height >> maxVal;
            file.ignore(); // Skip newline after maxVal

            ASSERT(header == "P6", "Unsupported PPM format!");

            // Read RGB pixel data
            std::vector<unsigned char> pixelDataRGB(width * height * 3);
            file.read(reinterpret_cast<char*>(pixelDataRGB.data()), pixelDataRGB.size());

            // Convert RGB to RGBA
            std::vector<unsigned char> pixelDataRGBA(width * height * 4);
            for (int i = 0, j = 0; i < pixelDataRGB.size(); i += 3, j += 4)
            {
                pixelDataRGBA[j] = pixelDataRGB[i];       // R
                pixelDataRGBA[j + 1] = pixelDataRGB[i + 1]; // G
                pixelDataRGBA[j + 2] = pixelDataRGB[i + 2]; // B
                pixelDataRGBA[j + 3] = 255;               // A (opaque)
            }

            // Create texture
            auto* texture = new Texture();
            texture->Initialize(width, height, Texture::Format::RGBA_U8, pixelDataRGBA.data());
            mTextures.push_back(texture);
        }
    }
}

void GameState::CreateFullscreenUISpriteWithTexture()
{
    // Create a new GameObject
    GameObject* gameObject = mGameWorld.CreateGameObject("FullscreenSprite");
    ASSERT(gameObject != nullptr, "Failed to create GameObject!");

    // Add ComponentUISprite to the GameObject
    auto* uiSprite = gameObject->AddComponent<ComponentUISprite>();
    ASSERT(uiSprite != nullptr, "Failed to add ComponentUISprite!");

    // Set the first frame texture as the initial texture
    if (!mTextures.empty())
    {
        uiSprite->SetTexture(mTextures[0]); // Use the first texture frame
    }

    // Add to the render service to ensure it renders last
    auto* renderService = mGameWorld.GetService<ServiceUIRender>();
    ASSERT(renderService != nullptr, "Render service not found!");
    renderService->Register(uiSprite);

    // Initialize the GameObject and its components
    gameObject->Initialize();

    // Store the handle if needed for later updates
    mFullscreenUISpriteHandle = gameObject->GetHandle();
}

void GameState::UpdateFullscreenUISpriteTexture()
{
    // Retrieve the GameObject for the fullscreen sprite
    GameObject* fullscreenSprite = mGameWorld.GetGameObject(mFullscreenUISpriteHandle);
    if (fullscreenSprite == nullptr || mTextures.empty())
        return;

    // Access the UISprite component
    auto* uiSprite = fullscreenSprite->GetComponent<ComponentUISprite>();
    if (uiSprite != nullptr)
    {
        // Set the current frame texture
        uiSprite->SetTexture(mTextures[mCurrentFrameIndex]);

        // Increment the frame index for the next update
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mTextures.size();
    }
}