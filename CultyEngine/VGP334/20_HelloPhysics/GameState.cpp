#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

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

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Mesh ball = MeshBuilder::CreateSphere(100, 100, 1.0f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapID = TextureManager::Get()->LoadTexture("misc/basketball.jpg");

    mBallShape.InitializeSphere(1.0f);
    mBallRB.Initialize(mBall.transform, mBallShape, 3.0f);

    Mesh ground = MeshBuilder::CreateHorizontalPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/concrete.jpg");
}

void GameState::Terminate()
{
    mGround.Terminate();

    mBallRB.Terminate();
    mBallShape.Terminate();
    mBall.Terminate();

    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    auto input = Input::InputSystem::Get();
    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        mBallRB.SetPosition({ 0.0f, 10.0f, 0.0f });
        mBallRB.SetVelocity({ 0.0f, 0.0f, 0.0f });
    }
}

void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mGround);
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
                mDirectionalLight.direction = MathC::Normalize(mDirectionalLight.direction);

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }

        mStandardEffect.DebugUI();
    ImGui::End();
}