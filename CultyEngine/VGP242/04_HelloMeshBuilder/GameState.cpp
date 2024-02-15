#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

void GameState::Initialize()
{
    // Set up Camera
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Create a shape
    //mMesh = MeshBuilder::CreateRectanglePC(2.0f);
    //mMesh = MeshBuilder::CreateCubePC(2.0f);
    //mMesh = MeshBuilder::CreateRectanglePC(2.0f, 0.5f, 1.0f);
    //mMesh = MeshBuilder::CreateVerticalPlanePC(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreateHorizontalPlanePC(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreateCylinderPC(10, 4);
    mMesh = MeshBuilder::CreateSpherePC(100.0f, 100.0f, 1.0f);

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

    mMeshBuffer.Initialize(mMesh);
    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    mVertexShader.Initialize<VertexPC>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);
}

void GameState::Terminate()
{
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
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

void GameState::Render()
{
    // Bind
    mVertexShader.Bind();
    mPixelShader.Bind();

    MathC::Matrix4 matWorld = MathC::Matrix4::Identity;
    MathC::Matrix4 matView = mCamera.GetViewMatrix();
    MathC::Matrix4 matProjection = mCamera.GetProjectionMatrix();
    MathC::Matrix4 matFinal = matWorld * matView * matProjection;
    MathC::Matrix4 wvp = MathC::Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mMeshBuffer.Render();
}