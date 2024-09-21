#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;
using namespace CultyEngine::Audio;
using namespace CultyEngine::Physics;

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
    mCamera.SetPosition({ -0.35f, 2.7f, -3.5f });
    mCamera.SetLookAt({ 0.5f, 1.0f, 0.5f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Mesh ground = MeshBuilder::CreateHorizontalPlane(100, 100, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

    mModelID_01 = ModelManager::Get()->LoadModelID("../../Assets/Models/Character02/Ch03_nonPBR.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Bashful.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/ArmsHipHopDance.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/SambaDancing.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/YmcaDance.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/KnockedOut.fbx");
    mCharacter_01 = CreateRenderGroup(mModelID_01, &mCharacterAnimator_01);
    mCharacterAnimator_01.Initialize(mModelID_01);

    mModelID_02 = ModelManager::Get()->LoadModelID("../../Assets/Models/Character03/Parasite_L_Starkie.fbx");
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/00_FlyingPunchCombo.fbx");    //0
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/04_BboyHipHopMove.fbx");      //1
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/05_DancingRunningMan.fbx");   //2
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/10_BreakdanceReady.fbx");     //3
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/11_BreakdanceUprock_01.fbx"); //4
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/12_BreakdanceUprock_02.fbx"); //5
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/17_FlairStart.fbx");          //6
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/18_FlairIn.fbx");             //7
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/19_FlairEnd.fbx");            //8
    ModelManager::Get()->AddAnimation(mModelID_02, "../../Assets/Models/Character03/Anims/20_AnnoyedHeadShake.fbx");    //9
    mCharacter_02 = CreateRenderGroup(mModelID_02, &mCharacterAnimator_02);
    mCharacterAnimator_02.Initialize(mModelID_02);

    mModelID_03 = ModelManager::Get()->LoadModelID("../../Assets/Models/Char04/character.fbx");
    ModelManager::Get()->AddAnimation(mModelID_03, "../../Assets/Models/Char04/Anims/Defeat.fbx");      //0
    ModelManager::Get()->AddAnimation(mModelID_03, "../../Assets/Models/Char04/Anims/Cheering.fbx");    //1
    mCharacter_03 = CreateRenderGroup(mModelID_03, &mCharacterAnimator_03);
    mCharacterAnimator_03.Initialize(mModelID_03);

    SoundEffectManager* sem = SoundEffectManager::Get();
    mEventSoundIDs.push_back(sem->Load("crowdShouting.wav"));
    mEventSoundIDs.push_back(sem->Load("largeCrowd.wav"));
    mEventSoundIDs.push_back(sem->Load("punch.wav"));

    AnimationCallback cb_sound_crowdShouting = [&]() { SoundEffectManager::Get()->Play(mEventSoundIDs[0]); };
    AnimationCallback cb_sound_crowdShoutingStop = [&]() { SoundEffectManager::Get()->Stop(mEventSoundIDs[0]); };
    AnimationCallback cb_sound_largeCrowd = [&]() { SoundEffectManager::Get()->Play(mEventSoundIDs[1]); };
    AnimationCallback cb_sound_largeCrowdStop = [&]() { SoundEffectManager::Get()->Stop(mEventSoundIDs[1]); };
    AnimationCallback cb_sound_punch = [&]() { SoundEffectManager::Get()->Play(mEventSoundIDs[2]); };

    AnimationCallback cb_char01_anim00 = [&]() {
        mCharacterAnimator_01.PlayAnimation(0, true);
        mStartTime = std::chrono::high_resolution_clock::now();
        };
    AnimationCallback cb_char01_anim01 = [&]() {mCharacterAnimator_01.PlayAnimation(1, true); };
    AnimationCallback cb_char01_anim02 = [&]() {mCharacterAnimator_01.PlayAnimation(2, true); };
    AnimationCallback cb_char01_anim03 = [&]() {mCharacterAnimator_01.PlayAnimation(3, true); };
    AnimationCallback cb_char01_anim04 = [&]() {mCharacterAnimator_01.PlayAnimation(4, false); };

    AnimationCallback cb_char02_anim00 = [&]() {mCharacterAnimator_02.PlayAnimation(0, false); };
    AnimationCallback cb_char02_anim01 = [&]() {mCharacterAnimator_02.PlayAnimation(1, true); };
    AnimationCallback cb_char02_anim02 = [&]() {mCharacterAnimator_02.PlayAnimation(2, true); };
    AnimationCallback cb_char02_anim03 = [&]() {mCharacterAnimator_02.PlayAnimation(3, true); };
    AnimationCallback cb_char02_anim04 = [&]() {mCharacterAnimator_02.PlayAnimation(4, true); };
    AnimationCallback cb_char02_anim05 = [&]() {mCharacterAnimator_02.PlayAnimation(5, true); };
    AnimationCallback cb_char02_anim06 = [&]() {mCharacterAnimator_02.PlayAnimation(6, true); };
    AnimationCallback cb_char02_anim07 = [&]() {mCharacterAnimator_02.PlayAnimation(7, true); };
    AnimationCallback cb_char02_anim08 = [&]() {mCharacterAnimator_02.PlayAnimation(8, true); };
    AnimationCallback cb_char02_anim09 = [&]() {mCharacterAnimator_02.PlayAnimation(9, true); };

    AnimationCallback cb_char03_anim00 = [&]() {mCharacterAnimator_03.PlayAnimation(0, true); };
    AnimationCallback cb_char03_anim01 = [&]() {mCharacterAnimator_03.PlayAnimation(1, true); };

    mEventAnimationTime = 0.0f;
    mEventAnimation_01 = AnimationBuilder()
        // Time: 00:00
        .AddPositionKey({0.0f, 0.0f, 0.0f}, 0.0f)
        .AddEventKey(cb_char01_anim00, 0.01f)
        .AddEventKey(cb_sound_crowdShouting, 1.0f)
        .AddEventKey(cb_char01_anim01, 4.9f)
        // Time: 05:00
        .AddEventKey(cb_char01_anim02, 9.9f)
        // Time: 10:00
        .AddPositionKey({0.0f, 0.0f, 0.0f}, 10.0f)
        .AddPositionKey({ 1.0f, 0.0f, 0.0f }, 12.0f)
        // Time: 15:00
        .AddPositionKey({ -1.0f, 0.0f, 0.0f }, 16.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 18.0f)
        .AddEventKey(cb_char01_anim03, 19.0f)
        // Time: 20:00
        .AddEventKey(cb_sound_punch, 24.85f)
        .AddEventKey(cb_sound_crowdShoutingStop, 24.9f)
        .AddEventKey(cb_char01_anim04, 24.99f)
        // Time: 25:00
        // Time: 45:00
        .AddPositionKey({0.0f, 0.0f, 0.0f}, 45.0f)
        .Build();

    mEventAnimation_02 = AnimationBuilder()
        // Time: 00:00
        .AddPositionKey({ 0.0f, 0.0f, 10.0f }, 0.0f)
        .AddEventKey(cb_char02_anim09, 0.01f)
        // Time: 05:00
        // Time: 15:00
        .AddPositionKey({ 0.0f, 0.0f, 10.0f }, 23.0f)
        // Time: 20:00
        .AddEventKey(cb_char02_anim00, 23.6f)
        .AddPositionKey({ 0.0f, 0.0f, 5.2f }, 24.9f)
        // Time: 25:00
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 25.0f)
        .AddEventKey(cb_char02_anim01, 25.0f)
        .AddEventKey(cb_sound_largeCrowd, 25.0f)
        .AddEventKey(cb_char02_anim02, 28.0f)
        // Time: 30:00
        .AddEventKey(cb_char02_anim03, 32.0f)
        .AddEventKey(cb_char02_anim04, 34.1f)
        // Time: 35:00
        .AddEventKey(cb_char02_anim05, 37.7)
        // Time: 40:00
        .AddEventKey(cb_char02_anim06, 40.1f)
        .AddEventKey(cb_char02_anim07, 41.95f)
        .AddEventKey(cb_char02_anim08, 43.0f)
        // Time: 45:00
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 45.0f)
        .AddEventKey(cb_sound_crowdShoutingStop, 45.0f)
        .Build();

    mEventAnimation_03 = AnimationBuilder()
        .AddEventKey(cb_char03_anim00, 0.01f)
        .AddEventKey(cb_char03_anim01, 25.5f)
        .Build();

    mParticleEffect.Initialize();
    mParticleEffect.SetCamera(mCamera);

    ParticleSystemInfo info;
    info.maxParticles = 200;
    info.particleTextureID = TextureManager::Get()->LoadTexture("../Images/white.jpg");
    info.spawnDirection = MathC::Vector3::YAxis;
    info.spawnDelay = 0.1f;
    info.spawnLifeTime = 999999999.0f;
    info.minParticlePerEmit = 5;
    info.maxParticlePerEmit = 10;
    info.minTimeBetweenEmit = 0.05f;
    info.maxTimeBetweenEmit = 0.10f;
    info.minSpawnAngle = -15.0f * MathC::Constants::Pi / 180.0f;
    info.maxSpawnAngle = 15.0f * MathC::Constants::Pi / 180.0f;
    info.minSpeed = 5.0f;
    info.maxSpeed = 10.0f;
    info.minParticleLifeTime = 1.0f;
    info.maxParticleLifeTime = 1.5f;
    info.minStartColor = Colors::Red;
    info.maxStartColor = Colors::Yellow;
    info.minEndColor = Colors::White;
    info.maxEndColor = Colors::Orange;
    info.minStartScale = { 0.25f, 0.25f, 0.25f };
    info.maxStartScale = { 0.5f, 0.5f, 0.5f };
    info.minEndScale = { 0.05f, 0.05f, 0.05f };
    info.maxEndScale = { 0.1f, 0.1f, 0.1f };

    info.spawnPosition = { -0.75, 0.0, 0.0 };
    mParticleSystem_01.Initialize(info);
    mParticleSystem_01.SetCamera(mCamera);

    info.spawnPosition = { 0.75, 0.0, 0.0 };
    mParticleSystem_02.Initialize(info);
    mParticleSystem_02.SetCamera(mCamera);
}

void GameState::Terminate()
{
    mParticleSystem_02.Terminate();
    mParticleSystem_01.Terminate();
    mParticleEffect.Terminate();

    CleanupRenderGroup(mCharacter_03);
    CleanupRenderGroup(mCharacter_02);
    CleanupRenderGroup(mCharacter_01);

    mGround.Terminate();

    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    auto input = Input::InputSystem::Get();
    
    CameraControl(deltaTime, mCamera);

    float prevTime = mEventAnimationTime;
    mEventAnimationTime += deltaTime;

    if (mEventAnimationTime >= mEventAnimation_02.GetDuration())
    {
        // Play Particles
        mParticleSystem_01.Update(deltaTime);
        mParticleSystem_02.Update(deltaTime);
        return;
    }

    mCharacterAnimator_01.Update(deltaTime);
    mEventAnimation_01.PlayEvents(prevTime, mEventAnimationTime);

    mCharacterAnimator_02.Update(deltaTime);
    mEventAnimation_02.PlayEvents(prevTime, mEventAnimationTime);

    mCharacterAnimator_03.Update(deltaTime);
    mEventAnimation_03.PlayEvents(prevTime, mEventAnimationTime);
}

void GameState::Render()
{
    // Apply Animation Transform to Character
    for (auto& ro : mCharacter_01)
        ro.transform = mEventAnimation_01.GetTransform(mEventAnimationTime);

    for (auto& ro : mCharacter_02)
        ro.transform = mEventAnimation_02.GetTransform(mEventAnimationTime);

    if (mEventAnimationTime >= mEventAnimation_02.GetDuration())
    {
        mParticleEffect.Begin();
            mParticleSystem_01.Render(mParticleEffect);
            mParticleSystem_02.Render(mParticleEffect);
        mParticleEffect.End();
    }

    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
        DrawRenderGroup(mStandardEffect, mCharacter_01);
        DrawRenderGroup(mStandardEffect, mCharacter_02);

        SetRenderGroupPosition(mCharacter_03, mTransform_01);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_02);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_03);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_04);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_05);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_06);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_07);
        DrawRenderGroup(mStandardEffect, mCharacter_03);
        SetRenderGroupPosition(mCharacter_03, mTransform_08);
        DrawRenderGroup(mStandardEffect, mCharacter_03);

    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ShowTimerDisplay();
    ImGui::End();
}

void GameState::ShowTimerDisplay()
{
    // Calculate the elapsed time in seconds
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - mStartTime;

    // Display the elapsed time in seconds
    float seconds = elapsedTime.count();
    ImGui::Text("Elapsed Time: %.2f seconds", seconds);
}