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
}

GameState::~GameState()
{
    delete(sun);
    delete(mercury);
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 50.0f, -100.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Set up Shader Data
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    sun = new Planet(L"../../Assets/Images/planets/sun.jpg", 10.0f, 0.0f);
    mercury = new Planet(L"../../Assets/Images/planets/mercury.jpg", 3.0f, 30.0f);
}

void GameState::Terminate()
{
    mSampler.Terminate();

    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();

    // Terminate Planet Data
    mercury->Terminate();
    sun->Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    sun->Update(deltaTime, 0.0f, 0.0f);
    mercury->Update(deltaTime, 1.0f, 0.0f);
}

void GameState::Render()
{
    // Bind
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    // Render each planet
    sun->Render(mCamera, mConstantBuffer);
    mercury->Render(mCamera, mConstantBuffer);
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

    ImGui::End();

    if (showGrid)
        SimpleDraw::AddGroundPlane(20, Colors::White);

    if (showTransform)
        SimpleDraw::AddTransform(MathC::Matrix4::Translation(transformPos));

    SimpleDraw::Render(mCamera);
}