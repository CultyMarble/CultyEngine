#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

void GameState::Initialize()
{
    // Set up Camera
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Create a shape PC
    //mMesh = MeshBuilder::CreateRectanglePC(2.0f);
    //mMesh = MeshBuilder::CreateCubePC(2.0f);
    //mMesh = MeshBuilder::CreateRectanglePC(2.0f, 0.5f, 1.0f);
    //mMesh = MeshBuilder::CreateVerticalPlanePC(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreateHorizontalPlanePC(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreateCylinderPC(10, 4);
    //mMesh = MeshBuilder::CreateSpherePC(10.0f, 10.0f, 1.0f);

    // Create a shape PX
    //mMesh = MeshBuilder::CreateHorizontalPlanePX(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreateSpherePX(100, 100, 1.0f);
    //mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 100.0f);
    mMesh = MeshBuilder::CreateSkyBoxPX(100.0f);

    // std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

    mMeshBuffer.Initialize(mMesh);
    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    //mVertexShader.Initialize<VertexPC>(shaderFilePath);
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    //mTexture.Initialize(L"../../Assets/Images/misc/basketball.jpg");
    mTexture.Initialize(L"../../Assets/Images/skybox/skybox_texture.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
    mSampler.Terminate();
    mTexture.Terminate();

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
    mTexture.BindPS(0);
    mSampler.BindPS(0);

    MathC::Matrix4 matWorld = MathC::Matrix4::Identity;
    MathC::Matrix4 matView = mCamera.GetViewMatrix();
    MathC::Matrix4 matProjection = mCamera.GetProjectionMatrix();
    MathC::Matrix4 matFinal = matWorld * matView * matProjection;
    MathC::Matrix4 wvp = MathC::Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mMeshBuffer.Render();
}