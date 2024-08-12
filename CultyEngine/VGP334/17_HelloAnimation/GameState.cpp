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

    Mesh ground = MeshBuilder::CreateHorizontalPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

    mAnimationTime = 0.0f;
    mAnimation = AnimationBuilder()

        // Falling
        .AddPositionKey({ 0.0f, 5.0f, 0.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
        .AddRotationKey({ 0.0f, 0.0f, 0.0f , 1.0 }, 0.0f)

        // Hit the ground
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.0f)
        .AddPositionKey({ 0.0f, 1.0f, 0.0f }, 1.0f, EaseType::EaseInQuad)

        // Swash down
        .AddPositionKey({ 0.0f, 0.85f, 0.0f }, 1.05f, EaseType::EaseInQuad)
        .AddPositionKey({ 0.0f, 0.75f, 0.0f }, 1.10f, EaseType::EaseInQuad)
        .AddPositionKey({ 0.0f, 0.65f, 0.0f }, 1.15f, EaseType::EaseInQuad)

        .AddPositionKey({ 0.0f, 0.7f, 0.0f }, 1.2f, EaseType::EaseInQuad)
        .AddScaleKey({ 1.0f, 0.8f, 1.0f }, 1.2f)
        .AddRotationKey({ 0.0f, 0.0f, -5.0f , 1.0 }, 0.0f)

        .AddPositionKey({ 0.0f, 0.65f, 0.0f }, 1.25f, EaseType::EaseOutQuad)
        .AddPositionKey({ 0.0f, 0.75f, 0.0f }, 1.30f, EaseType::EaseOutQuad)
        .AddPositionKey({ 0.0f, 0.85f, 0.0f }, 1.35f, EaseType::EaseOutQuad)

        // Pounce back
        .AddPositionKey({ 0.0f, 1.0f, 0.0f }, 1.4f, EaseType::EaseOutQuad)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.4f)

        .AddPositionKey({ 0.0f, 5.0f, 0.0f }, 2.0f, EaseType::EaseOutQuad)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 2.0f)
        .AddRotationKey({ 0.0, 0.0f, -6.0f , 1.0 }, 2.0f)

        .Build();
}

void GameState::Terminate()
{
    mBall.Terminate();
    mGround.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    // Update Animation
    // Loop Animation
    mAnimationTime += deltaTime;
    while (mAnimationTime > mAnimation.GetDuration())
        mAnimationTime -= mAnimation.GetDuration();
}

void GameState::Render()
{
    mBall.transform = mAnimation.GetTransform(mAnimationTime);

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