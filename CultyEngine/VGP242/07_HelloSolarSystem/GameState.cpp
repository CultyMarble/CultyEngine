#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

namespace
{
    void CameraControl(float deltaTime, Camera& mCamera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 50.0f;
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

    void CreateMeshData()
    {
        //mMesh = MeshBuilder::CreateCubePC(2.0f);
        //mMesh = MeshBuilder::CreateRectanglePC(2.0f, 0.5f, 1.0f);
        //mMesh = MeshBuilder::CreateVerticalPlanePC(5, 7, 1.0f);
        //mMesh = MeshBuilder::CreateHorizontalPlanePC(8, 3, 1.0f);
        //mMesh = MeshBuilder::CreateSpherePC(100.0f, 100.0f, 1.0f);
        //mMesh = MeshBuilder::CreateCylinderPC(100, 2);

        //mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 100.0f);
        //mTexture.Initialize(L"../../Assets/Images/skysphere/space.jpg");

        //std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 50.0f, -100.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Create Mesh Data
    mMesh_Sun = MeshBuilder::CreateSpherePX(100, 100, 10.0f);

    // Set up Shader Data
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

    mMeshBuffer.Initialize(mMesh_Sun);
    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    // Create Texture Data
    mTexture.Initialize(L"../../Assets/Images/planets/sun.jpg");

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
    CameraControl(deltaTime, mCamera);

    // Planet X
    SpherePos.x = SPHERE_REVOLUTION_RADIUS;
    SpherePos.z = sqrt(Sqr(SPHERE_REVOLUTION_RADIUS) - Sqr(SpherePos.x));

    /*if (moveForward)
        SpherePos.x += SPHERE_MOVE_SPEED_X * deltaTime;
    else
        SpherePos.x -= SPHERE_MOVE_SPEED_X * deltaTime;

    if (SpherePos.x >= SPHERE_REVOLUTION_RADIUS)
    {
        moveForward = false;
        if (upperHalf)
            upperHalf = false;
    }
    else if (SpherePos.x <= -SPHERE_REVOLUTION_RADIUS)
    {
        if (moveForward == false && upperHalf == false)
            upperHalf = true;

        moveForward = true;
    }

    if (upperHalf)
    {
        SpherePos.z = sqrt(Sqr(SPHERE_REVOLUTION_RADIUS) - Sqr(SpherePos.x));
    }
    else
    {
        SpherePos.z = -sqrt(Sqr(SPHERE_REVOLUTION_RADIUS) - Sqr(SpherePos.x));;
    }*/

    deltaRotate += rotateSpeed * deltaTime;
    if (deltaRotate >= 360.0f)
        deltaRotate -= 360.0f;
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

namespace
{
    bool showGrid = false;
    bool showTransform = false;

    CultyEngine::MathC::Vector3 transformPos = CultyEngine::MathC::Vector3::Zero;
}

void GameState::DebugUI()
{
    ImGui::Begin("DebugUI: Hello Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        DebugUI::SetTheme(DebugUI::Theme::Dark);

        if (ImGui::CollapsingHeader("Gizmo", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Show Grid", &showGrid);
            ImGui::Checkbox("Show Transform", &showTransform);
            ImGui::DragFloat3("Transform Position", &transformPos.x, 0.01f);
        }

        if (ImGui::CollapsingHeader("Gizmo", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Transform Position", &SpherePos.x, 0.01f);
        }

    ImGui::End();

    SimpleDraw::AddSphere(20, 20, 2.5f, Colors::Pink, MathC::Matrix4::Translation(SpherePos), deltaRotate);
    SimpleDraw::AddGroundCircle(50, SPHERE_REVOLUTION_RADIUS, Colors::White);

    if (showGrid)
        SimpleDraw::AddGroundPlane(20, Colors::White);

    if (showTransform)
        SimpleDraw::AddTransform(MathC::Matrix4::Translation(transformPos));

    SimpleDraw::Render(mCamera);
}