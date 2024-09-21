#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    void OnEvent2();
    void OnEvent3();

    void OnSpaceEvent(const CultyEngine::Event* event);
    void OnAnimEvent(const CultyEngine::Event* event);

    CultyEngine::Graphics::DirectionalLight mDirectionalLight;
    CultyEngine::Graphics::Camera mCamera;

    CultyEngine::Graphics::ModelID mModelID_01;
    CultyEngine::Graphics::RenderGroup mCharacter_01;
    CultyEngine::Graphics::Animator mCharacterAnimator_01;
    CultyEngine::Graphics::StandardEffect mStandardEffect;

    CultyEngine::ListenerID mSpaceEventID = 0;
    CultyEngine::ListenerID mAnimEventID = 0;

    std::vector<CultyEngine::Audio::SoundId> mEventSoundIDs;
    CultyEngine::Graphics::Animation mEventAnimation_01;
    float mEventAnimationTime = 0.0f;

    int mAnimationIndex = -1;
    bool mDrawSkeleton{};
};