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
    mCamera.SetPosition({ 0.0f, 2.0f, -2.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    mModelID01 = ModelManager::Get()->LoadModelID("../../Assets/Models/Character03/Parasite_L_Starkie.fbx");
    mCharacter01 = CreateRenderGroup(mModelID01);
    SetRenderGroupPosition(mCharacter01, { -1.0f, 0.0f, 0.0f });

    mModelID02 = ModelManager::Get()->LoadModelID("../../Assets/Models/Character02/Ch03_nonPBR.fbx");
    mCharacter02 = CreateRenderGroup(mModelID02);
    SetRenderGroupPosition(mCharacter02, { 1.0f, 0.0f, 0.0f });

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    CleanupRenderGroup(mCharacter02);
    CleanupRenderGroup(mCharacter01);
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);
}

void GameState::Render()
{
    if (mDrawSkeleton)
    {
        AnimationUtils::BoneTransforms boneTransforms;
        AnimationUtils::ComputeBoneTransforms(mModelID01, boneTransforms);
        AnimationUtils::DrawSkeleton(mModelID01, boneTransforms);

        AnimationUtils::ComputeBoneTransforms(mModelID02, boneTransforms);
        AnimationUtils::DrawSkeleton(mModelID02, boneTransforms);
    }

    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    if (mDrawSkeleton == false)
    {
        mStandardEffect.Begin();
            DrawRenderGroup(mStandardEffect, mCharacter01);
            DrawRenderGroup(mStandardEffect, mCharacter02);
        mStandardEffect.End();
    }
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

        ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
        mStandardEffect.DebugUI();
    ImGui::End();
}