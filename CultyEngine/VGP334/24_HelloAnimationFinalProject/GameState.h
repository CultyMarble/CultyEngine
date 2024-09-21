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

    CultyEngine::Graphics::ModelID mModelID_01;
    CultyEngine::Graphics::RenderGroup mCharacter_01;
    CultyEngine::Graphics::Animator mCharacterAnimator_01;
    CultyEngine::Graphics::Animation mEventAnimation_01;

    CultyEngine::Graphics::ModelID mModelID_02;
    CultyEngine::Graphics::RenderGroup mCharacter_02;
    CultyEngine::Graphics::Animator mCharacterAnimator_02;
    CultyEngine::Graphics::Animation mEventAnimation_02;

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