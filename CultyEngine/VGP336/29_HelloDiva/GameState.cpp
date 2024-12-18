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

    void LoadTimeline(NoteList& mNotes, const std::filesystem::path& timelineFile)
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

    void SpawnNotes(NoteList& mNotes, float& mElapsedTime, CultyEngine::GameWorld& mGameWorld)
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
            mNotes.erase(mNotes.begin());
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
    SpawnNotes(mNotes, mElapsedTime, mGameWorld);

    mGameWorld.Update(deltaTime);

    auto inputSystem = Input::InputSystem::Get();

    if (inputSystem->IsControllerButtonDown(Input::PS4_BUTTON_X))
        LOG("Test -- Controller: Button X Pressed!");

    if (inputSystem->IsControllerButtonDown(Input::PS4_BUTTON_CIRCLE))
        LOG("Test -- Controller: Button Circle Pressed!");

    if (inputSystem->IsControllerButtonDown(Input::PS4_BUTTON_SQUARE))
        LOG("Test -- Controller: Button Square Pressed!");

    if (inputSystem->IsControllerButtonDown(Input::PS4_BUTTON_TRIANGLE))
        LOG("Test -- Controller: Button Triangle Pressed!");
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