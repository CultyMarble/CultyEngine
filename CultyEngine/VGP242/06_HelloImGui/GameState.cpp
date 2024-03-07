#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

const char* shapeTypeChar[]
{
    "Sphere",
    "AABB",
    "FilledAABB",
    "Circle",
};

enum class ShapeType
{
    Sphere,
    AABB,
    FilledAABB,
    Circle,
};

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

Color shapeColor = Colors::Orange;
ShapeType shapeType = ShapeType::Sphere;
int currentShapeIndex = (int)shapeType;
bool showGrid = true;
bool showTransform = true;
float mShapeAlpha = 1.0f;
float sphereRadius = 1.0f;
float circleRadius = 1.0f;
void GameState::DebugUI()
{
    ImGui::Begin("DebugUI: ImGui Configs", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::LabelText("Title", "Hello Simple Draw!");

        if (ImGui::CollapsingHeader("Theme", ImGuiTreeNodeFlags_CollapsingHeader))
        {
            if (ImGui::Button("Classic"))
                DebugUI::SetTheme(DebugUI::Theme::Classic);

            if (ImGui::Button("Dark"))
                DebugUI::SetTheme(DebugUI::Theme::Dark);

            if (ImGui::Button("Flashbang"))
                DebugUI::SetTheme(DebugUI::Theme::Light);
        }

        if (ImGui::CollapsingHeader("Gizmo Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Show Grid", &showGrid);
            ImGui::Checkbox("Show Transform", &showTransform);
            ImGui::DragFloat3("Transform Position", &transformPos.x, 0.01f);
        }

        if (ImGui::CollapsingHeader("Shape Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            currentShapeIndex = (int)shapeType;
            if (ImGui::Combo("Shape Type", &currentShapeIndex, shapeTypeChar, 4))
            {
                shapeType = (ShapeType)currentShapeIndex;
            }

            ImGui::ColorEdit4("Shape Color", &shapeColor.r);
            ImGui::DragFloat("Shape Alpha", &mShapeAlpha, 0.001f, 0.0f, 1.0f);

            ImGui::DragFloat("Sphere Radius", &sphereRadius, 0.1f, 0.1f, 5.0f);
            ImGui::DragFloat("Circle Radius", &circleRadius, 0.1f, 0.1f, 5.0f);
        }

    ImGui::End();

    shapeColor.a = mShapeAlpha;

    switch (shapeType)
    {
    case ShapeType::Sphere:
        SimpleDraw::AddSphere(50, 50, sphereRadius, shapeColor);
        break;
    case ShapeType::AABB:
        SimpleDraw::AddAABB(-MathC::Vector3::One, MathC::Vector3::One, shapeColor);
        break;
    case ShapeType::FilledAABB:
        SimpleDraw::AddFilledAABB(-MathC::Vector3::One, MathC::Vector3::One, shapeColor);
        break;
    case ShapeType::Circle:
        SimpleDraw::AddGroundCircle(60, circleRadius, shapeColor);
        break;
    default:
        break;
    }

    if (showGrid)
        SimpleDraw::AddGroundPlane(20, Colors::White);

    if (showTransform)
        SimpleDraw::AddTransform(MathC::Matrix4::Translation(transformPos));

    SimpleDraw::Render(mCamera);
}