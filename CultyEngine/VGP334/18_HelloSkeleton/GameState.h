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

    CultyEngine::Graphics::ModelID mModelID;
    CultyEngine::Graphics::RenderGroup mCharacter;

    CultyEngine::Graphics::StandardEffect mStandardEffect;

    bool mDrawSkeleton{};
};