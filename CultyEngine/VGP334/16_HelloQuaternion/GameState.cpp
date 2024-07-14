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
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    Model model;

    ModelIO::LoadModel("../../Assets/Models/Character01/Paladin_J_Nordstrom.model", model);
    ModelIO::LoadMaterial("../../Assets/Models/Character01/Paladin_J_Nordstrom.model", model);
    mCharacter01 = CreateRenderGroup(model);
    SetRenderGroupPosition(mCharacter01, { 0.0f, 0.0f, 0.0f });

    ModelIO::LoadModel("../../Assets/Models/Character02/Ch03_nonPBR.model", model);
    ModelIO::LoadMaterial("../../Assets/Models/Character02/Ch03_nonPBR.model", model);
    mCharacter02 = CreateRenderGroup(model);
    SetRenderGroupPosition(mCharacter02, { -1.5f, 0.0f, 0.0f });

    ModelIO::LoadModel("../../Assets/Models/Character03/Parasite_L_Starkie.fbx", model);
    ModelIO::LoadMaterial("../../Assets/Models/Character03/Parasite_L_Starkie.fbx", model);
    mCharacter03 = CreateRenderGroup(model);
    SetRenderGroupPosition(mCharacter03, { 1.5f, 0.0f, 0.0f });

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();

    CleanupRenderGroup(mCharacter03);
    CleanupRenderGroup(mCharacter02);
    CleanupRenderGroup(mCharacter01);
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);
}

void GameState::Render()
{
    //SimpleDraw::AddTransform(mTransform-)
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        DrawRenderGroup(mStandardEffect, mCharacter01);
        DrawRenderGroup(mStandardEffect, mCharacter02);
        DrawRenderGroup(mStandardEffect, mCharacter03);
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
            {
                mDirectionalLight.direction = MathC::Normalize(mDirectionalLight.direction);
            }

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }
        bool updateRotation = false;
        if (ImGui::DragFloat("PlayerYaw", &mYaw, 0.01f))
            updateRotation = true;
        if (ImGui::DragFloat("PlayerPitch", &mPitch, 0.01f))
            updateRotation = true;
        if (ImGui::DragFloat("PlayerRoll", &mRoll, 0.01f))
            updateRotation = true;

        mStandardEffect.DebugUI();
    ImGui::End();

    if (updateRotation)
    {
        Quaternion q = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
        for (auto& ro : mCharacter01)
            ro.transform.rotation = q;
        for (auto& ro : mCharacter02)
            ro.transform.rotation = q;
        for (auto& ro : mCharacter03)
            ro.transform.rotation = q;

        mTransform.rotation = q;
    }
}