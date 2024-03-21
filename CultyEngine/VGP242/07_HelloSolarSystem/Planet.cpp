#include "Planet.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

Planet::Planet(std::filesystem::path texturePath, float radius, float distance)
{
    mPosition = { 0.0f, 0.0f, distance };
    localDirection = { 0.0f, 0.0f, 1.0f };
    worldDirection = { 0.0f, 0.0f, 1.0f };

    // Create Mesh Data
    mMesh = MeshBuilder::CreateSpherePX(100, 100, radius);
    mMeshBuffer.Initialize(mMesh);

    // Create Texture Data
    mTexture.Initialize(texturePath);
}

void Planet::Terminate()
{
    mTexture.Terminate();
    mMeshBuffer.Terminate();
}

void Planet::Update(float deltaTime, float revolveSpeed, float rotateSpeed)
{
    // World View Rotate
    Matrix4 worldRotation = Matrix4::RotationY(1.0f * revolveSpeed * deltaTime);
    worldDirection = TransformNormal(worldDirection, worldRotation);

    // Local View Rotate
    Matrix4 localRotation = Matrix4::RotationY(1.0f * rotateSpeed * deltaTime);
    localDirection = TransformNormal(localDirection, localRotation);
}

void Planet::Render(Camera& mCamera, ConstantBuffer& mConstantBuffer)
{
    mTexture.BindPS(0);
    Vector3 center = CultyEngine::MathC::Vector3::Zero;

    MathC::Matrix4 worldView = GetViewMatrix(localDirection, center);
    MathC::Matrix4 localView = GetViewMatrix(worldDirection, mPosition);

    MathC::Matrix4 matView = mCamera.GetViewMatrix();
    MathC::Matrix4 matProjection = mCamera.GetProjectionMatrix();

    MathC::Matrix4 matFinal = worldView * localView * matView * matProjection;
    MathC::Matrix4 wvp = MathC::Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mMeshBuffer.Render();
}

Matrix4 Planet::GetViewMatrix(CultyEngine::MathC::Vector3& direction, CultyEngine::MathC::Vector3& viewPosition)
{
    const MathC::Vector3 l = direction;
    const MathC::Vector3 r = MathC::Normalize(MathC::Cross(MathC::Vector3::YAxis, l));
    const MathC::Vector3 u = MathC::Normalize(MathC::Cross(l, r));
    const float x = -MathC::Dot(r, viewPosition);
    const float y = -MathC::Dot(u, viewPosition);
    const float z = -MathC::Dot(l, viewPosition);

    return
    {
        r.x, u.x, l.x, 0.0f,
        r.y, u.y, l.y, 0.0f,
        r.z, u.z, l.z, 0.0f,
        x,   y,   z,   1.0f
    };
}