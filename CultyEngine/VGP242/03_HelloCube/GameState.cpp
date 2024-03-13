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
    mMeshPX.vertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::Red });
    mMeshPX.vertices.push_back({ { -0.5f,  0.5f, 0.0f }, Colors::Blue });
    mMeshPX.vertices.push_back({ {  0.5f,  0.5f, 0.0f }, Colors::Green });
    mMeshPX.vertices.push_back({ {  0.5f, -0.5f, 0.0f }, Colors::Yellow });

    mMeshPX.vertices.push_back({ { -0.5f, -0.5f, 1.0f }, Colors::Red });
    mMeshPX.vertices.push_back({ { -0.5f,  0.5f, 1.0f }, Colors::Blue });
    mMeshPX.vertices.push_back({ {  0.5f,  0.5f, 1.0f }, Colors::Green });
    mMeshPX.vertices.push_back({ {  0.5f, -0.5f, 1.0f }, Colors::Yellow });

    mMeshPX.indices = { 
        // front
        0, 1, 2, 
        0, 2, 3,
        // back
        7, 5, 4,
        7, 6, 5,
        // right
        3, 2, 6,
        3, 6, 7,
        // left
        4, 5, 1,
        4, 1, 0,
        // top
        1, 5, 6,
        1, 6, 2,
        // bottom
        0, 3, 7,
        0, 7, 4,
    };

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

    mMeshBuffer.Initialize(mMeshPX);
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