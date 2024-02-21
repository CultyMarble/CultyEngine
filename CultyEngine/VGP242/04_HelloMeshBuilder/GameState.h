#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize() override;

    void Terminate() override;

    void Update(float deltaTime) override;

    void Render() override;

private:
    CultyEngine::Graphics::Camera mCamera;
    CultyEngine::Graphics::ConstantBuffer mConstantBuffer;

    //CultyEngine::Graphics::MeshPC mMesh;
    CultyEngine::Graphics::MeshPX mMesh;
    CultyEngine::Graphics::MeshBuffer mMeshBuffer;
    CultyEngine::Graphics::VertexShader mVertexShader;
    CultyEngine::Graphics::PixelShader mPixelShader;

    CultyEngine::Graphics::Texture mTexture;
    CultyEngine::Graphics::Sampler mSampler;
};