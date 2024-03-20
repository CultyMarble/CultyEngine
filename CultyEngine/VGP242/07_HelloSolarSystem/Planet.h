#pragma once

#include <CultyEngine/Inc/CultyEngine.h>

class Planet
{
public:
    Planet(std::filesystem::path texturePath, float radius, float distance);
    void Terminate();
    void Update(float deltaTime, float revolveSpeed, float rotateSpeed);
    void Render(CultyEngine::Graphics::Camera& mCamera, CultyEngine::Graphics::ConstantBuffer& mConstantBuffer);

protected:
    Matrix4 GetLocalViewMatrix();
    Matrix4 GetWorldViewMatrix();

    CultyEngine::Graphics::MeshPX mMesh;
    CultyEngine::Graphics::MeshBuffer mMeshBuffer;

    CultyEngine::Graphics::Texture mTexture;

    std::filesystem::path shaderFilePath;

    CultyEngine::MathC::Vector3 localDirection = {};
    CultyEngine::MathC::Vector3 worldDirection = {};
    CultyEngine::MathC::Vector3 mPosition = {};
};
