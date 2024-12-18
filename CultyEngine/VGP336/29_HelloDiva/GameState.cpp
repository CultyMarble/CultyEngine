#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>

#include "GameState.h"
#include "ComponentNoteMovement.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Audio;

namespace
{
    using NoteList = std::vector<std::tuple<float, std::string, std::string>>;
    using NoteQueue = std::queue<CultyEngine::GameObjectHandle>;

    void LoadTimeline(NoteList& mNotes,const std::filesystem::path& timelineFile)
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
}

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
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    LoadTimeline(mNotes, L"../../Assets/Templates/ProjectDiva/gameplay_timeline.json");

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
    GameObject* trackedObject = mGameWorld.GetGameObject(mTrackedNote);
    if (!trackedObject)
        return;

    auto* noteMovement = trackedObject->GetComponent<ComponentNoteMovement>();
    if (!noteMovement || !noteMovement->IsActive())
        return;

    auto* inputSystem = Input::InputSystem::Get();
    for (int button : mValidButtons)
    {
        if (inputSystem->IsControllerButtonDown(button) == false)
            continue;

        float timeRemaining = noteMovement->TimeRemaining();
        float totalTime = noteMovement->GetTotalTime();

        if (timeRemaining > totalTime * 0.1f)
            continue;

        if (noteMovement->IsCorrectButton(button))
        {
            if (timeRemaining <= totalTime * 0.02f)
                LOG("Hit: Cool (Golden)");
            else if (timeRemaining <= totalTime * 0.03f)
                LOG("Hit: Cool (Normal)");
            else if (timeRemaining <= totalTime * 0.05f)
                LOG("Hit: Fine");
            else if (timeRemaining <= totalTime * 0.07f)
                LOG("Hit: Safe");
            else if (timeRemaining <= totalTime * 0.10f)
                LOG("Hit: Worst");

            mGameWorld.GetGameObject(mTrackedNote)->GetComponent<ComponentNoteMovement>()->Terminate();
            mTrackedNote = CultyEngine::GameObjectHandle();
            break;
        }
        else
        {
            LOG("Miss: Incorrect button.");
            mGameWorld.GetGameObject(mTrackedNote)->GetComponent<ComponentNoteMovement>()->Terminate();
            mTrackedNote = CultyEngine::GameObjectHandle();
            break;
        }
    }
}