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

    void StateControl()
    {
        if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
        {
            MainApplication().ChangeState("StateCube");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
        {
            MainApplication().ChangeState("StateRect");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
        {
            MainApplication().ChangeState("StateVertPlane");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
        {
            MainApplication().ChangeState("StateHorzPlane");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
        {
            MainApplication().ChangeState("StateSphere");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
        {
            MainApplication().ChangeState("StateCylinder");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
        {
            MainApplication().ChangeState("StateSkyBox");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::EIGHT))
        {
            MainApplication().ChangeState("StateSkySphere");
        }
    }
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
    CameraControl(deltaTime, mCamera);

    StateControl();
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

void StateMeshPC::Initialize()
{
    // Set up Camera
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Create Mesh Data
    CreateMeshData();

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

    mMeshBuffer.Initialize(mMesh);
    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    mVertexShader.Initialize<VertexPC>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StateMeshPX::Initialize()
{
    // Set up Camera
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Create Mesh Data
    CreateMeshData();

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

    mMeshBuffer.Initialize(mMesh);
    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    // Create Texture Data
    CreateTextureData();

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

// MESH PC
void StateCube::CreateMeshData()
{
    mMesh = MeshBuilder::CreateCubePC(2.0f);
}

void StateRect::CreateMeshData()
{
    mMesh = MeshBuilder::CreateRectanglePC(2.0f, 0.5f, 1.0f);
}

void StateVertPlane::CreateMeshData()
{
    mMesh = MeshBuilder::CreateVerticalPlanePC(5, 7, 1.0f);
}

void StateHorzPlane::CreateMeshData()
{
    mMesh = MeshBuilder::CreateHorizontalPlanePC(8, 3, 1.0f);
}

void StateSphere::CreateMeshData()
{
    mMesh = MeshBuilder::CreateSpherePC(100.0f, 100.0f, 1.0f);
}

void StateCylinder::CreateMeshData()
{
    mMesh = MeshBuilder::CreateCylinderPC(100, 2);
}

// MESH PX
void StateSkyBox::CreateMeshData()
{
    mMesh = MeshBuilder::CreateSkyBoxPX(100.0f);
}

void StateSkyBox::CreateTextureData()
{
    mTexture.Initialize(L"../../Assets/Images/skybox/skybox_texture.jpg");
}

void StateSkySphere::CreateMeshData()
{
    mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 100.0f);
}

void StateSkySphere::CreateTextureData()
{
    mTexture.Initialize(L"../../Assets/Images/skysphere/space.jpg");
}