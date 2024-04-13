#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mVertexShader.Initialize<Vertex>(filePath);
	mPixelShader.Initialize(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect: No Camera Set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	mSettingsBuffer.BindPS(1);
}

void StandardEffect::End()
{
	// nothing until shadow
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	const MathC::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const MathC::Matrix4 matView = mCamera->GetViewMatrix();
	const MathC::Matrix4 matProj = mCamera->GetProjectionMatrix();

	MathC::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = MathC::Transpose(matFinal);
	mTransformBuffer.Update(transformData);

	SettingsData settingsData;
	settingsData.useDiffuseMap = renderObject.diffuseTextureID > 0 && 
		mSettingsData.useDiffuseMap > 0 ? 1 : 0;
	mSettingsBuffer.Update(settingsData);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseTextureID, 0);
	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
		{
			mSettingsData.useDiffuseMap = useDiffuseMap ? 1 : 0;
		}
	}
}
