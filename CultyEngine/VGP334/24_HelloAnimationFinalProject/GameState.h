#pragma once
#include <CultyEngine/Inc/CultyEngine.h>
#include <chrono>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    void ShowTimerDisplay();

    CultyEngine::Graphics::DirectionalLight mDirectionalLight;
    CultyEngine::Graphics::Camera mCamera;
    CultyEngine::Graphics::StandardEffect mStandardEffect;

    CultyEngine::Graphics::RenderObject mGround;

    CultyEngine::Graphics::ModelID mModelID_01;
    CultyEngine::Graphics::RenderGroup mCharacter_01;
    CultyEngine::Graphics::Animator mCharacterAnimator_01;
    CultyEngine::Graphics::Animation mEventAnimation_01;

    CultyEngine::Graphics::ModelID mModelID_02;
    CultyEngine::Graphics::RenderGroup mCharacter_02;
    CultyEngine::Graphics::Animator mCharacterAnimator_02;
    CultyEngine::Graphics::Animation mEventAnimation_02;

    CultyEngine::Graphics::ModelID mModelID_03;
    CultyEngine::Graphics::RenderGroup mCharacter_03;
    CultyEngine::Graphics::Animator mCharacterAnimator_03;
    CultyEngine::Graphics::Animation mEventAnimation_03;
    CultyEngine::MathC::Vector3 mTransform_00 = { 0.0f, 0.0f, 2.0f };
    CultyEngine::MathC::Vector3 mTransform_01 = { 0.5f, 0.0f, 3.0f };
    CultyEngine::MathC::Vector3 mTransform_02 = { 1.0f, 0.0f, 2.0f };
    CultyEngine::MathC::Vector3 mTransform_03 = { 1.5f, 0.0f, 3.5f };
    CultyEngine::MathC::Vector3 mTransform_04 = { 2.0f, 0.0f, 2.0f };
    CultyEngine::MathC::Vector3 mTransform_05 = { -0.5f, 0.0f, 2.3f };
    CultyEngine::MathC::Vector3 mTransform_06 = { -1.0f, 0.0f, 2.7f };
    CultyEngine::MathC::Vector3 mTransform_07 = { -1.5f, 0.0f, 1.9f };
    CultyEngine::MathC::Vector3 mTransform_08 = { -2.0f, 0.0f, 3.2f };

    CultyEngine::ListenerID mSpaceEventID = 0;
    CultyEngine::ListenerID mAnimEventID = 0;

    std::vector<CultyEngine::Audio::SoundId> mEventSoundIDs;
    float mEventAnimationTime = 0.0f;

    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime = {};

    // Particle System
    CultyEngine::Graphics::ParticleSystemEffect mParticleEffect;
    CultyEngine::Physics::ParticleSystem mParticleSystem_01;
    CultyEngine::Physics::ParticleSystem mParticleSystem_02;
};