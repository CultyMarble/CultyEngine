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
    CultyEngine::Graphics::DirectionalLight mDirectionalLight;
    CultyEngine::Graphics::Camera mCamera;

    CultyEngine::Graphics::ModelID mModelID_01;
    CultyEngine::Graphics::RenderGroup mCharacter_01;
    CultyEngine::Graphics::Animator mCharacterAnimator_01;

    CultyEngine::Graphics::StandardEffect mStandardEffect;

    int mAnimationIndex = -1;
    bool mDrawSkeleton{};
};