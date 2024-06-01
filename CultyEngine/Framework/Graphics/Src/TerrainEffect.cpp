#include "Precompiled.h"
#include "TerrainEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "VertexTypes.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void TerrainEffect::Initialize()
{
	std::filesystem::path shaderFile = "../../Assets/Shaders/Terrain.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
	mSampler.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void TerrainEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindPS(0);

	mSettingsBuffer.BindVS(1);
	mSettingsBuffer.BindPS(1);

	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindPS(3);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void TerrainEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void TerrainEffect::Render(const RenderObject& renderObject)
{
}

void TerrainEffect::DebugUI()
{
}

void TerrainEffect::SetCamera(const Camera& camera)
{
}

void TerrainEffect::SetLightCamera(const Camera& camera)
{
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
}

void TerrainEffect::SetShadowMap(const Texture& shadowMap)
{
}
