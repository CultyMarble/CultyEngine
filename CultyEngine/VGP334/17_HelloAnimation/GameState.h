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

    CultyEngine::Graphics::StandardEffect mStandardEffect;

    CultyEngine::Graphics::RenderObject mGround;
    CultyEngine::Graphics::RenderObject mBall;

    CultyEngine::Graphics::Animation mAnimation;
    float mAnimationTime = 0.0f;
};