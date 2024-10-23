#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

void FPSCameraComponent::Initialize()
{
    mCameraComponent = GetOwner().GetComponent<CameraComponent>();
    ASSERT(mCameraComponent != nullptr, "FPSCameraComponent: Camera not found");
}

void FPSCameraComponent::Terminate()
{
    mCameraComponent = nullptr;
}

void FPSCameraComponent::Update(float deltaTime)
{
    Camera& camera = mCameraComponent->GetCamera();
    auto input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
        camera.Walk(moveSpeed * deltaTime);
    else if (input->IsKeyDown(KeyCode::S))
        camera.Walk(-moveSpeed * deltaTime);

    if (input->IsKeyDown(KeyCode::A))
        camera.Strafe(-moveSpeed * deltaTime);
    else if (input->IsKeyDown(KeyCode::D))
        camera.Strafe(moveSpeed * deltaTime);

    if (input->IsKeyDown(KeyCode::Q))
        camera.Rise(-moveSpeed * deltaTime);
    else if (input->IsKeyDown(KeyCode::E))
        camera.Rise(moveSpeed * deltaTime);

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}