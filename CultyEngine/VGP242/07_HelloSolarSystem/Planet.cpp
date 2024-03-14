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

void Planet::Update(float deltaTime, float rotateSpeed, float revolveSpeed)
{
	// Local Rotate
	Matrix4 localRotation = Matrix4::RotationY(1.0f * rotateSpeed * deltaTime);
	localDirection = TransformNormal(localDirection, localRotation);

	// World Rotate
	Matrix4 worldRotation = Matrix4::RotationY(1.0f * revolveSpeed * deltaTime);
	worldDirection = TransformNormal(worldDirection, worldRotation);
}

void Planet::Render(Camera& mCamera, ConstantBuffer& mConstantBuffer)
{
	mTexture.BindPS(0);

	MathC::Matrix4 worldView = GetWorldViewMatrix();
	MathC::Matrix4 localView = GetLocalViewMatrix();

	MathC::Matrix4 matView = mCamera.GetViewMatrix();
	MathC::Matrix4 matProjection = mCamera.GetProjectionMatrix();

	MathC::Matrix4 matFinal = worldView * localView * matView * matProjection;
	MathC::Matrix4 wvp = MathC::Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}

Matrix4 Planet::GetLocalViewMatrix()
{
	const MathC::Vector3 l = localDirection;
	const MathC::Vector3 r = MathC::Normalize(MathC::Cross(MathC::Vector3::YAxis, l));
	const MathC::Vector3 u = MathC::Normalize(MathC::Cross(l, r));
	const float x = -MathC::Dot(r, mPosition);
	const float y = -MathC::Dot(u, mPosition);
	const float z = -MathC::Dot(l, mPosition);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}

Matrix4 Planet::GetWorldViewMatrix()
{
	MathC::Vector3 position = { 0.0f, 0.0f, 0.0f };

	const MathC::Vector3 l = worldDirection;
	const MathC::Vector3 r = MathC::Normalize(MathC::Cross(MathC::Vector3::YAxis, l));
	const MathC::Vector3 u = MathC::Normalize(MathC::Cross(l, r));
	const float x = -MathC::Dot(r, position);
	const float y = -MathC::Dot(u, position);
	const float z = -MathC::Dot(l, position);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}
