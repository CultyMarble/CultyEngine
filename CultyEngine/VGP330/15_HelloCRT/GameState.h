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

    // Effects
    CultyEngine::Graphics::StandardEffect mStandardEffect;
    CultyEngine::Graphics::CRTEffect mCRTEffect;

    // Objects
    CultyEngine::Graphics::RenderObject mGround;
    CultyEngine::Graphics::RenderGroup mCharacter03;
    CultyEngine::Graphics::RenderObject mScreenQuad;
    CultyEngine::Graphics::Texture mCombinedTexture;

    // Render Targets
    CultyEngine::Graphics::RenderTarget mRenderTarget;
};