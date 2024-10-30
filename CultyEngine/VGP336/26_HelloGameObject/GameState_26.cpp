#include "GameState_26.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void GameState::Initialize()
{
    mGameWorld.AddService<ServiceCamera>();
    mGameWorld.Initialize();

    GameObject* camera = mGameWorld.CreateGameObject("FPSCamera");
    camera->AddComponent<ComponentCamera>();
    camera->AddComponent<ComponentFPSCamera>();
    camera->Initialize();

    GameObject* gameObject = mGameWorld.CreateGameObject("Object0");
    gameObject->AddComponent<ComponentTransform>();
    gameObject->Initialize();
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