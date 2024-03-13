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
    virtual void CreateMeshData() {};
    virtual void CreateTextureData() {};

    CultyEngine::Graphics::Camera mCamera;

    CultyEngine::Graphics::MeshPX mMesh_Sun;
    CultyEngine::Graphics::ConstantBuffer mConstantBuffer;
    CultyEngine::Graphics::MeshBuffer mMeshBuffer;
    CultyEngine::Graphics::VertexShader mVertexShader;
    CultyEngine::Graphics::PixelShader mPixelShader;

    CultyEngine::Graphics::Texture mTexture;
    CultyEngine::Graphics::Sampler mSampler;

    CultyEngine::Graphics::MeshPX mMesh_Mercury;

    // TEST PLANET
    CultyEngine::MathC::Vector3 SpherePos = CultyEngine::MathC::Vector3::Zero;
    bool moveForward = true;
    bool upperHalf = true;
    const float SPHERE_MOVE_SPEED_X = 20.0f;
    const float SPHERE_REVOLUTION_RADIUS = 25.0f;

    float deltaRotate = {};
    float rotateSpeed = 10.0f;
};