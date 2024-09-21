#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;
using namespace CultyEngine::Audio;

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
    mCamera.SetPosition({ 0.0f, 2.0f, -2.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    mModelID_01 = ModelManager::Get()->LoadModelID("../../Assets/Models/Character02/Ch03_nonPBR.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Bashful.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Bboy.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Capoeira.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Flair.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/SoulSpinCombo.fbx");
    mCharacter_01 = CreateRenderGroup(mModelID_01, &mCharacterAnimator_01);
    mCharacterAnimator_01.Initialize(mModelID_01);
    SetRenderGroupPosition(mCharacter_01, { 0.0f, 0.5f, 0.0f });

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    SoundEffectManager* sem = SoundEffectManager::Get();
    mEventSoundIDs.push_back(sem->Load("megamanx_blast.wav"));
    mEventSoundIDs.push_back(sem->Load("photongun1.wav"));
    mEventSoundIDs.push_back(sem->Load("explosion.wav"));

    AnimationCallback cb = [&]()
        { 
            SoundEffectManager::Get()->Play(mEventSoundIDs[0]);
            mCharacterAnimator_01.PlayAnimation(1, true);
        };

    mEventAnimationTime = 0.0f;
    mEventAnimation_01 = AnimationBuilder()
        .AddPositionKey({0.0f, 0.0f, 0.0f}, 0.0f)
        .AddPositionKey({3.0f, 0.0f, 0.0f}, 2.0f)
        .AddPositionKey({0.0f, 0.0f, 0.0f}, 3.0f)
        .AddEventKey(cb, 1.0f)
        .AddEventKey(std::bind(&GameState::OnEvent2, this), 2.0f)
        .AddEventKey(std::bind(&GameState::OnEvent3, this), 3.0f)
        .Build();

    EventManager* em = EventManager::Get();
    mSpaceEventID = em->AddListener(EventType::SpacePressed, std::bind(&GameState::OnSpaceEvent, this, std::placeholders::_1));
    mAnimEventID = em->AddListener(EventType::AnimEvent, std::bind(&GameState::OnAnimEvent, this, std::placeholders::_1));
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    CleanupRenderGroup(mCharacter_01);
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    mCharacterAnimator_01.Update(deltaTime);

    // if character1 and character2 overlap
    // ContactEvent contactEvent;
    //      contactEvent.objA = character1;
    //      contactEvent.objB = character2;
    // EventManager::Broadcast(contactEvent);


    auto input = Input::InputSystem::Get();
    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        SpacePressedEvent spacePressed;
        EventManager::Broadcast(&spacePressed);
    }

    float prevTime = mEventAnimationTime;
    mEventAnimationTime += deltaTime;
    mEventAnimation_01.PlayEvents(prevTime, mEventAnimationTime);
    while (mEventAnimationTime >= mEventAnimation_01.GetDuration())
    {
        mEventAnimationTime -= mEventAnimation_01.GetDuration();
    }
}

void GameState::Render()
{
    for (auto& ro : mCharacter_01)
    {
        ro.transform = mEventAnimation_01.GetTransform(mEventAnimationTime);
    }

    if (mDrawSkeleton)
    {
        AnimationUtils::BoneTransforms boneTransforms;

        AnimationUtils::ComputeBoneTransforms(mModelID_01, boneTransforms, &mCharacterAnimator_01);
        AnimationUtils::DrawSkeleton(mModelID_01, boneTransforms);
    }

    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    if (mDrawSkeleton == false)
    {
        mStandardEffect.Begin();
            DrawRenderGroup(mStandardEffect, mCharacter_01);
        mStandardEffect.End();
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
                mDirectionalLight.direction = MathC::Normalize(mDirectionalLight.direction);

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }

        ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);

        if (ImGui::DragInt("Animation", &mAnimationIndex, 1, -1, mCharacterAnimator_01.GetAnimationCount() - 1))
            mCharacterAnimator_01.PlayAnimation(mAnimationIndex, true);

        mStandardEffect.DebugUI();
    ImGui::End();
}

void GameState::OnEvent2()
{
    SoundEffectManager::Get()->Play(mEventSoundIDs[1]);
    AnimEvent animEvent;
    animEvent.eventName = "Shoot";
    EventManager::Broadcast(&animEvent);
}

void GameState::OnEvent3()
{
    SoundEffectManager::Get()->Play(mEventSoundIDs[2]);
    AnimEvent animEvent;
    animEvent.eventName = "Explode";
    EventManager::Broadcast(&animEvent);
}

void GameState::OnSpaceEvent(const CultyEngine::Event* event)
{
    LOG("SPACE EVENT OCCURRED");
    SoundEffectManager::Get()->Play(mEventSoundIDs[2]);
}

void GameState::OnAnimEvent(const CultyEngine::Event* event)
{
    AnimEvent* eventData = (AnimEvent*)event;
    if (eventData->eventName == "Shoot")
    {
        LOG("SHOOT EVENT OCCURRED");
    }
    else if (eventData->eventName == "Explode")
    {
        LOG("EXPLODE EVENT OCCURRED!!!!");
    }
}