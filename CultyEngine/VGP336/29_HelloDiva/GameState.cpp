#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>

#include "GameState.h"
#include "ComponentNoteMovement.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Audio;

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "ComponentNoteMovement")
        return gameObject.AddComponent<ComponentNoteMovement>();
    return nullptr;
}

Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "ComponentNoteMovement")
        return gameObject.GetComponent<ComponentNoteMovement>();
    return nullptr;
}

void GameState::Initialize()
{
    // Load sound effects
    auto* soundManager = CultyEngine::Audio::SoundEffectManager::Get();
    mSoundIdHit_Cool = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/hit_cool.wav");
    mSoundIdHit_Empty = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/hit_empty.wav");
    mSoundIdHit_Regular = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/hit_regular.wav");
    mSoundIdMiss = soundManager->Load(L"../../Assets/Sounds/ProjectDiva/miss.wav");

    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    LoadTimeline(L"../../Assets/Templates/ProjectDiva/gameplay_timeline.json");

    mGameWorld.LoadLevel(L"../../Assets/Templates/ProjectDiva/gameplay_level.json");
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mElapsedTime += deltaTime;
    SpawnNote();

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
        auto* noteMovement = noteObject->GetComponent<ComponentNoteMovement>();
        noteMovement->SetOnDestroyedCallback([this](CultyEngine::GameObjectHandle handle)
            {
                if (handle.GetIndex() == mTrackedNote.GetIndex())
                {
                    LOG("Miss: Note disappeared.");
                    mTrackedNote = CultyEngine::GameObjectHandle(); // Clear the tracked note
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

    auto* noteMovement = trackedObject->GetComponent<ComponentNoteMovement>();
    if (noteMovement == nullptr)
        return;

    // Activate the note if it's within the active zone
    if (!noteMovement->IsActive() && noteMovement->TimeRemaining() <= noteMovement->GetTotalTime() * 0.4f)
        noteMovement->SetActive(true);

    // Handle note disappearance
    if (noteMovement->HasDisappeared())
    {
        // Raise a miss and clear the tracked note
        LOG("Miss: Note disappeared.");
        mTrackedNote = CultyEngine::GameObjectHandle();
    }
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
    if (!trackedObject || !trackedObject->GetComponent<ComponentNoteMovement>()->IsActive())
    {
        if (isAnyButtonPressed)
            soundManager->Play(mSoundIdHit_Empty);
        return;
    }

    auto* noteMovement = trackedObject->GetComponent<ComponentNoteMovement>();

    // Process player input for the tracked note
    for (int button : mValidButtons)
    {
        if (!inputSystem->IsControllerButtonDown(button))
            continue;

        float timeRemaining = noteMovement->TimeRemaining();
        float totalTime = noteMovement->GetTotalTime();

        // Process hits or misses within the active timing window
        if (timeRemaining <= totalTime * 0.1f)
        {
            if (!noteMovement->IsCorrectButtonDown(button))
            {
                // Miss: Incorrect button
                LOG("Miss: Incorrect button.");
                soundManager->Play(mSoundIdMiss);
                noteMovement->Terminate();
                mTrackedNote = CultyEngine::GameObjectHandle();
                return;
            }

            // Determine hit quality and play corresponding sound
            if (timeRemaining <= totalTime * 0.02f)
            {
                LOG("Hit: Cool (Golden)");
                soundManager->Play(mSoundIdHit_Cool);
            }
            else if (timeRemaining <= totalTime * 0.03f)
            {
                LOG("Hit: Cool (Normal)");
                soundManager->Play(mSoundIdHit_Regular);
            }
            else if (timeRemaining <= totalTime * 0.04f)
            {
                LOG("Hit: Fine");
                soundManager->Play(mSoundIdHit_Regular);
            }
            else if (timeRemaining <= totalTime * 0.05f)
            {
                LOG("Hit: Safe");
                soundManager->Play(mSoundIdHit_Regular);
            }
            else if (timeRemaining <= totalTime * 0.07f)
            {
                LOG("Hit: Worst");
                soundManager->Play(mSoundIdHit_Regular);
            }
            else
            {
                LOG("Hit: Miss");
                soundManager->Play(mSoundIdMiss);
            }

            // Terminate the note after processing a hit
            noteMovement->Terminate();
            mTrackedNote = CultyEngine::GameObjectHandle();
            return;
        }
    }

    // Play empty hit sound for valid button presses outside active timing window
    if (isAnyButtonPressed)
        soundManager->Play(mSoundIdHit_Empty);
}