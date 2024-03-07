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
            if (MainApplication().IsStateActive("StateCube"))
                return;
            MainApplication().ChangeState("StateCube");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
        {
            if (MainApplication().IsStateActive("StateGroundPlane"))
                return;
            MainApplication().ChangeState("StateGroundPlane");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
        {
            if (MainApplication().IsStateActive("StateSphere"))
                return;
            MainApplication().ChangeState("StateSphere");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
        {
            if (MainApplication().IsStateActive("StateAABB"))
                return;
            MainApplication().ChangeState("StateAABB");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
        {
            if (MainApplication().IsStateActive("StateFilledAABB"))
                return;
            MainApplication().ChangeState("StateFilledAABB");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
        {
            if (MainApplication().IsStateActive("StateLines"))
                return;
            MainApplication().ChangeState("StateLines");
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

    StateControl();
}

void StateTransform::Render()
{
    SimpleDraw::Render(mCamera);
    SimpleDraw::AddTransform(MathC::Matrix4::Identity);
}

void StateGroundPlane::Render()
{
    SimpleDraw::Render(mCamera);
    SimpleDraw::AddGroundPlane(100, Colors::White);
}

void StateSphere::Render()
{
    SimpleDraw::Render(mCamera);
    SimpleDraw::AddSphere(50, 50, 10, Colors::Pink);
}

void StateAABB::Render()
{
    SimpleDraw::Render(mCamera);
    SimpleDraw::AddAABB(-MathC::Vector3::One, MathC::Vector3::One, Colors::Coral);
}

void StateFilledAABB::Render()
{
    SimpleDraw::Render(mCamera);
    SimpleDraw::AddFilledAABB(-MathC::Vector3::One, MathC::Vector3::One, Colors::Gray);
}

void StateLines::Render()
{
    SimpleDraw::Render(mCamera);

    // Bottom
    SimpleDraw::AddLine({ -1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, Colors::Yellow);
    SimpleDraw::AddLine({ 1.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, Colors::Purple);
    SimpleDraw::AddLine({ 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, -1.0f }, Colors::Pink);

    // Side -> Top
    SimpleDraw::AddLine({ -1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Red);
    SimpleDraw::AddLine({ 1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Green);
    SimpleDraw::AddLine({ 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Blue);
}