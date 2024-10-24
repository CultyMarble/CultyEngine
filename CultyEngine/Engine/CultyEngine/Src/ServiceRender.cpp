#include "Precompiled.h"
#include "ServiceRender.h"

#include "CameraService.h"
#include "ComponentRenderObject.h"
#include "TransformComponent.h"

#include "GameWorld.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ServiceRender::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f , 1.0f };

	mStandardEffect.Initialize();
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
}

void ServiceRender::Terminate()
{
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void ServiceRender::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void ServiceRender::Render()
{
	const Camera& camera = mCameraService->GetMain();
	mStandardEffect.SetCamera(camera);
	for (Entry& entry : mRenderEntries)
	{
		for (RenderObject& renderObject : entry.renderGroup)
		{
			renderObject.transform = *entry.transformComponent;
		}
	}

	mShadowEffect.Begin();
	for (Entry* entry : mRenderEntries)
	{
		if (entry->renderComponent->CanCastShadow())
		{
			DrawRenderGroup(mShadowEffect, entry.renderGroup);
		}
	}
	mShadowEffect.End();

	mStandardEffect.Begin();
	for (Entry* entry : mRenderEntries)
	{
		DrawRenderGroup(mShadowEffect, entry.renderGroup);
	}
	mStandardEffect.End();
}

void ServiceRender::DebugUI()
{
	ImGui::Text("FPS: %f", mFPS);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();
}

void ServiceRender::Register(const ComponentRenderObject* componentRenderObject)
{
	Entry& entry = mRenderEntries.emplace_back();
	entry.renderComponent = componentRenderObject;
	entry.transformComponent = componentRenderObject->GetOwner().GetComponent<TransformComponent>();

	if (componentRenderObject->GetModelID() > 0)
	{
		entry.renderGroup = CreateRenderGroup(componentRenderObject->GetModelID);
	}
	else
	{
		entry.renderGroup = CreateRenderGroup(componentRenderObject->GetModelID());
	}
}

void ServiceRender::Unregister(const ComponentRenderObject* componentRenderObject)
{
	auto iter = std::find_if(
		mRenderEntries.begin(), mRenderEntries.end(),
		[&](const)
	)
}
