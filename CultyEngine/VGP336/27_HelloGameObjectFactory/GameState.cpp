#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.Initialize();

    mGameWorld.CreateGameObject("FPSCamera", "../../Assets/Templates/fps_camera.json");
    mGameWorld.CreateGameObject("Object0", "../../Assets/Templates/test_object.json");
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