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

    // create a shape
    TextureManager* tm = TextureManager::Get();
    mMesh = MeshBuilder::CreateSphere(100, 100, 1.0f);

    //mRenderObject1.meshBuffer.Initialize(mMesh);
    //mRenderObject1.diffuseTextureID = tm->LoadTexture("planets/earth/earth.jpg");
    //mRenderObject1.transform.position.x = 0.0f;

    int numberOfObjects = 3;
    for (int i = 0; i < numberOfObjects; ++i)
    {
        RenderObject& newObject = mRenderObject.emplace_back();
        newObject.meshBuffer.Initialize(mMesh);
        newObject.diffuseTextureID = tm->LoadTexture("planets/earth/earth.jpg");
        newObject.transform.position.x = float(i);
    }
    
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();

    //mRenderObject1.Terminate();

    for (auto& r : mRenderObject)
        r.Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        //mStandardEffect.Render(mRenderObject1);

        for (auto& r : mRenderObject)
            mStandardEffect.Render(r);

    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mStandardEffect.DebugUI();
    ImGui::End();
}