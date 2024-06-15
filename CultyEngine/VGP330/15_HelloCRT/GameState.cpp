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
    // Camera
    mCamera.SetPosition({ 0.0f, 2.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 3.0f });

    // Light
    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Objects
    Mesh groundMesh = MeshBuilder::CreateHorizontalPlane(4, 4, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/paper.jpg");

    Model model;
    ModelIO::LoadModel("../../Assets/Models/Character02/Ch03_nonPBR.fbx", model);
    ModelIO::LoadMaterial("../../Assets/Models/Character02/Ch03_nonPBR.fbx", model);
    mCharacter03 = CreateRenderGroup(model);

    MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
    mScreenQuad.meshBuffer.Initialize(screenQuad);

    // Effect
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    shaderFilePath = L"../../Assets/Shaders/CRT.fx";
    mCRTEffect.Initialize(shaderFilePath);
    mCRTEffect.SetTexture(&mRenderTarget);
    mCRTEffect.SetTexture(&mCombinedTexture, 1);

    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenWidth = gs->GetBackBufferWidth() / 2;
    const uint32_t screenHeight = gs->GetBackBufferHeight() / 2;
    mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

    mCombinedTexture.Initialize("../../Assets/Images/water/water_spec.jpg");
}

void GameState::Terminate()
{
    mCombinedTexture.Terminate();

    // RenderTarget
    mRenderTarget.Terminate();

    // Effect
    mCRTEffect.Terminate();
    mStandardEffect.Terminate();

    // Mesh Stuffs
    mScreenQuad.Terminate();
    CleanupRenderGroup(mCharacter03);
    mGround.Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    // Extra
    mCRTEffect.Update(deltaTime);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mRenderTarget.BeginRender();
        mStandardEffect.Begin();
            mStandardEffect.Render(mGround);
            DrawRenderGroup(mStandardEffect, mCharacter03);
        mStandardEffect.End();
    mRenderTarget.EndRender();

    mCRTEffect.Begin();
        mCRTEffect.Render(mScreenQuad);
    mCRTEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Separator();
        ImGui::Text("Render Target:");
        ImGui::Image(
            mRenderTarget.GetRawData(),
            { 128, 128 },
            { 0, 0 },
            { 1 ,1 },
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 }
        );

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

        mStandardEffect.DebugUI();
        mCRTEffect.DebugUI();

    ImGui::End();
}