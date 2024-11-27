#include "GameState.h"

#include "ComponentCustomDebugDraw.h"
#include "ServiceCustomDebugDraw.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;
using namespace CultyEngine::Audio;

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "ComponentCustomDebugDraw")
        return gameObject.AddComponent<ComponentCustomDebugDraw>();

    return nullptr;
}
Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "ComponentCustomDebugDraw")
        return gameObject.GetComponent<ComponentCustomDebugDraw>();

    return nullptr;
}
Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameworld)
{
    if (serviceName == "ServiceCustomDebugDraw")
        return gameworld.AddService<ServiceCustomDebugDraw>();

    return nullptr;
}

void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);
    GameWorld::SetCustomService(CustomServiceMake);
    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/test_level.json");
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
    // TO DO
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mGameWorld.DebugUI();
    ImGui::End();
}