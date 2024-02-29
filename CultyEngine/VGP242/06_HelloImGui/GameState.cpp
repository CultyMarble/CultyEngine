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
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);
}

void GameState::Render()
{

}

bool buttonOn = false;
void GameState::DebugUI()
{
    ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::LabelText("Title", "Hello World!");
    if (ImGui::Button("Button"))
    {
        buttonOn = !buttonOn;
    }
    if (buttonOn)
    {

    }

    ImGui::DragFloat("SphereAlpha", &mSphereAlpha, 0.1f, 0.0f, 1.0f);
    ImGui::DragFloat3("TransformPos", &mPosition.x);
    ImGui::End();

    SimpleDraw::AddTransform(MathC::Matrix4::Identity);
    SimpleDraw::AddGroundPlane(100, Colors::White);
    SimpleDraw::AddSphere(50, 50, 10, Colors::Pink);
    SimpleDraw::Render(mCamera);
}