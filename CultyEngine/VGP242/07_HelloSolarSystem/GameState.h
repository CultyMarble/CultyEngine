#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

#include "Planet.h"

class GameState : public CultyEngine::ApplicationState
{
public:
    ~GameState();

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    virtual void CreateMeshData() {};
    virtual void CreateTextureData() {};

    CultyEngine::Graphics::Camera mCamera;

    CultyEngine::Graphics::ConstantBuffer mConstantBuffer;
    CultyEngine::Graphics::VertexShader mVertexShader;
    CultyEngine::Graphics::PixelShader mPixelShader;

    CultyEngine::Graphics::Sampler mSampler;

    // Create skySphere
    CultyEngine::Graphics::MeshBuffer mMeshBuffer;
    CultyEngine::Graphics::MeshPX mGalaxyBG;
    CultyEngine::Graphics::Texture mGalaxyTexture;

    // Create Planets
    Planet* sun;
    Planet* mercury;
    Planet* venus;
    Planet* earth;
    Planet* mars;
    Planet* jupiter;
    Planet* saturn;
    Planet* uranus;
    Planet* neptune;
    Planet* pluto;
};