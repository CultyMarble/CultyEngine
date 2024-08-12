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

    CultyEngine::Graphics::ModelID mModelID01;
    CultyEngine::Graphics::ModelID mModelID02;
    CultyEngine::Graphics::RenderGroup mCharacter01;
    CultyEngine::Graphics::RenderGroup mCharacter02;

    CultyEngine::Graphics::StandardEffect mStandardEffect;

    bool mDrawSkeleton{};
};