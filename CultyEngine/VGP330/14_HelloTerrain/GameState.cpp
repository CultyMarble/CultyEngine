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
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Light
    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Objects
    Model model;
    ModelIO::LoadModel("../../Assets/Models/Character03/Parasite_L_Starkie.fbx", model);
    ModelIO::LoadMaterial("../../Assets/Models/Character03/Parasite_L_Starkie.fbx", model);
    mCharacter03 = CreateRenderGroup(model);

    mTerrain.Initialize("../../Assets/Images/terrain/heightmap_512x512.raw", 20.0f);
    const Mesh& m = mTerrain.GetMesh();

    mGround.meshBuffer.Initialize(
        nullptr,
        static_cast<uint32_t>(sizeof(Vertex)),
        static_cast<uint32_t>(m.vertices.size()),
        m.indices.data(),
        static_cast<uint32_t>(m.indices.size()));

    mGround.meshBuffer.Update(m.vertices.data(), m.vertices.size());
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("terrain/grass_2048.jpg");
    mGround.bumpMapID = TextureManager::Get()->LoadTexture("terrain/dirt_seamless.jpg");

    // Effect
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mTerrainEffect.Initialize();
    mTerrainEffect.SetCamera(mCamera);
    mTerrainEffect.SetDirectionalLight(mDirectionalLight);
    mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    // Effect
    mShadowEffect.Terminate();
    mTerrainEffect.Terminate();
    mStandardEffect.Terminate();

    // Mesh Stuffs
    mGround.Terminate();
    CleanupRenderGroup(mCharacter03);
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    if (mOnTerrain)
    {
        Vector3 pos = mCamera.GetPosition();
        float height = mTerrain.GetHeight(pos);
        pos.y = height + 0.5f;
        mCamera.SetPosition(pos);
    }
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mShadowEffect.SetFocus(mCamera.GetPosition());

    mShadowEffect.Begin();
        DrawRenderGroup(mShadowEffect, mCharacter03);
    mShadowEffect.End();

    mTerrainEffect.Begin();
        mTerrainEffect.Render(mGround);
    mTerrainEffect.End();

    mStandardEffect.Begin();
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

        if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("OnTerrain", &mOnTerrain);
            float heightScale = mTerrain.GetHeightScale();
            if (ImGui::DragFloat("HeightScale", &heightScale, 0.1f, 1.0f, 100.0f))
            {
                mTerrain.SetHeightScale(heightScale);
                const Mesh& m = mTerrain.GetMesh();
                mGround.meshBuffer.Update(m.vertices.data(), m.vertices.size());
            }
        }

        mStandardEffect.DebugUI();
        mTerrainEffect.DebugUI();
        mShadowEffect.DebugUI();

    ImGui::End();
}