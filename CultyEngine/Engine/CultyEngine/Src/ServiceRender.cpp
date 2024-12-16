#include "Precompiled.h"
#include "ServiceRender.h"

#include "ServiceCamera.h"
#include "ComponentAnimator.h"
#include "ComponentRenderObject.h"
#include "ComponentTransform.h"

#include "GameWorld.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ServiceRender::Initialize()
{
    mCameraService = GetWorld().GetService<ServiceCamera>();

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f , 1.0f };

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);
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
            renderObject.transform = entry.componentTransform->GetWorldTransform();
        }
    }

    mShadowEffect.Begin();
    for (Entry& entry : mRenderEntries)
    {
        if (entry.componentRender->CanCastShadow())
        {
            DrawRenderGroup(mShadowEffect, entry.renderGroup);
        }
    }
    mShadowEffect.End();

    mStandardEffect.Begin();
    for (Entry& entry : mRenderEntries)
    {
        DrawRenderGroup(mStandardEffect, entry.renderGroup);
    }
    mStandardEffect.End();
}

void ServiceRender::DebugUI()
{
    if (ImGui::CollapsingHeader("Rendering"))
    {
        ImGui::Text("FPS: %f", mFPS);
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
                mDirectionalLight.direction = Normalize(mDirectionalLight.direction);

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }
        mStandardEffect.DebugUI();
        mShadowEffect.DebugUI();
    }
}

void ServiceRender::Register(const ComponentRenderObject* componentRenderObject)
{
    Entry& entry = mRenderEntries.emplace_back();
    entry.componentRender = componentRenderObject;
    entry.componentTransform = componentRenderObject->GetOwner().GetComponent<ComponentTransform>();

    const ComponentAnimator* componentAnimator = componentRenderObject->GetOwner().GetComponent<ComponentAnimator>();
    const Animator* animator = nullptr;
    if (componentAnimator != nullptr)
        animator = &componentAnimator->GetAnimator();

    if (componentRenderObject->GetModelID() > 0)
    {
        entry.renderGroup = CreateRenderGroup(componentRenderObject->GetModelID(), animator);
    }
    else
    {
        entry.renderGroup = CreateRenderGroup(componentRenderObject->GetModel());
    }
}

void ServiceRender::Unregister(const ComponentRenderObject* componentRenderObject)
{
    auto iter = std::find_if
    (
        mRenderEntries.begin(),
        mRenderEntries.end(),
        [&](const Entry& entry) { return entry.componentRender == componentRenderObject; }
    );

    if (iter != mRenderEntries.end())
    {
        CleanupRenderGroup(iter->renderGroup);
        mRenderEntries.erase(iter);
    }
}
