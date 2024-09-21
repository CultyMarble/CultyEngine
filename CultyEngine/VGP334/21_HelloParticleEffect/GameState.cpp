#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;
using namespace CultyEngine::Physics;

namespace
{
    void CameraControl(float deltaTime, Camera& mCamera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
        const float turnSpeed = 0.1f;

        if (input->IsKeyDown(KeyCode::W))
            mCamera.Walk(moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::S))
            mCamera.Walk(-moveSpeed * deltaTime);

        if (input->IsKeyDown(KeyCode::A))
            mCamera.Strafe(-moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::D))
            mCamera.Strafe(moveSpeed * deltaTime);

        if (input->IsKeyDown(KeyCode::Q))
            mCamera.Rise(-moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::E))
            mCamera.Rise(moveSpeed * deltaTime);

        if (input->IsMouseDown(MouseButton::RBUTTON))
        {
            mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
            mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
        }
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    mParticleEffect.Initialize();
    mParticleEffect.SetCamera(mCamera);

    ParticleSystemInfo info;
    info.maxParticles = 100;
    info.particleTextureID = TextureManager::Get()->LoadTexture("../Images/white.jpg");
    info.spawnPosition = MathC::Vector3::Zero;
    info.spawnDirection = MathC::Vector3::YAxis;
    info.spawnDelay = 0.1f;
    info.spawnLifeTime = 999999999.0f;
    info.minParticlePerEmit = 3;
    info.maxParticlePerEmit = 5;
    info.minTimeBetweenEmit = 0.05f;
    info.maxTimeBetweenEmit = 0.15f;
    info.minSpawnAngle = -30.0f * MathC::Constants::Pi / 180.0f;
    info.maxSpawnAngle =  30.0f * MathC::Constants::Pi / 180.0f;
    info.minSpeed = 2.0f;
    info.maxSpeed = 5.0f;
    info.minParticleLifeTime = 0.5f;
    info.maxParticleLifeTime = 1.0f;
    info.minStartColor = Colors::Red;
    info.maxStartColor = Colors::Yellow;
    info.minEndColor = Colors::White;
    info.maxEndColor = Colors::Orange;
    info.minStartScale = MathC::Vector3::One;
    info.maxStartScale = { 1.5f, 1.5f, 1.5f };
    info.minEndScale = { 0.05f, 0.05f, 0.05f };
    info.maxEndScale = { 0.1f, 0.1f, 0.1f };

    mParticleSystem_01.Initialize(info);
    mParticleSystem_01.SetCamera(mCamera);
}

void GameState::Terminate()
{
    mParticleSystem_01.Terminate();
    mParticleEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    auto input = Input::InputSystem::Get();

    mParticleSystem_01.Update(deltaTime);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(20.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mParticleEffect.Begin();
        mParticleSystem_01.Render(mParticleEffect);
    mParticleEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mParticleEffect.DebugUI();
        mParticleSystem_01.DebugUI();
        Physics::PhysicsWorld::Get()->DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCamera);
}