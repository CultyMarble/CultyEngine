#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Terminate() override;

    void Update(float deltaTime) override;

    void Render() override;

protected:
    virtual void CreateMeshData() {};
    virtual void CreateTextureData() {};

    CultyEngine::Graphics::Camera mCamera;
    CultyEngine::Graphics::ConstantBuffer mConstantBuffer;

    CultyEngine::Graphics::MeshBuffer mMeshBuffer;
    CultyEngine::Graphics::VertexShader mVertexShader;
    CultyEngine::Graphics::PixelShader mPixelShader;

    CultyEngine::Graphics::Texture mTexture;
    CultyEngine::Graphics::Sampler mSampler;
};

/// <summary>
/// State Class
class StateMeshPC : public GameState
{
public:
    void Initialize() override;

protected:
    CultyEngine::Graphics::MeshPC mMesh;
};

class StateMeshPX : public GameState
{
public:
    void Initialize() override;

protected:
    CultyEngine::Graphics::MeshPX mMesh;
};

/// <summary>
/// Mesh Class
class StateCube : public StateMeshPC
{
protected:
    void CreateMeshData() override;
};

class StateRect : public StateMeshPC
{
protected:
    void CreateMeshData() override;
};

class StateVertPlane : public StateMeshPC
{
protected:
    void CreateMeshData() override;
};

class StateHorzPlane : public StateMeshPC
{
protected:
    void CreateMeshData() override;
};

class StateSphere : public StateMeshPC
{
protected:
    void CreateMeshData() override;
};

class StateCylinder : public StateMeshPC
{
protected:
    void CreateMeshData() override;
};

class StateSpherePX : public StateMeshPX
{
protected:
    void CreateMeshData() override;
    void CreateTextureData() override;
};

class StateSkyBox : public StateMeshPX
{
protected:
    void CreateMeshData() override;
    void CreateTextureData() override;
};

class StateSkySphere : public StateMeshPX
{
protected:
    void CreateMeshData() override;
    void CreateTextureData() override;
};