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
    CultyEngine::Graphics::Camera mCamera;
    CultyEngine::Graphics::Mesh mMesh;

    // CultyEngine::Graphics::RenderObject mRenderObject1;

    std::vector<CultyEngine::Graphics::RenderObject> mRenderObject;

    CultyEngine::Graphics::StandardEffect mStandardEffect;
};