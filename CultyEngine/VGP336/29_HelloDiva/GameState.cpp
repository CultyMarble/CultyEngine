#include "GameState.h"
#include "ComponentNoteMovement.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;
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
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    mGameWorld.LoadLevel(L"../../Assets/Templates/ProjectDiva/gameplay_level.json");
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mGameWorld.DebugUI();
    ImGui::End();
}