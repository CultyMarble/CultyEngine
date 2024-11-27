#include "Precompiled.h"
#include "ServiceUIRender.h"

#include "ComponentUI.h";

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ServiceUIRender::Initialize()
{
}

void ServiceUIRender::Terminate()
{
    mComponentUIs.clear();
}

void ServiceUIRender::Update(float deltaTime)
{
    for (ComponentUI* componentUI : mComponentUIs)
        componentUI->Update(deltaTime);
}

void ServiceUIRender::Render()
{
    UISpriteRenderer::Get()->BeginRender();

    for (ComponentUI* componentUI : mComponentUIs)
        componentUI->Render();

    UISpriteRenderer::Get()->EndRender();
}

void ServiceUIRender::Register(ComponentUI* componentUI)
{
    auto iter = std::find(mComponentUIs.begin(), mComponentUIs.end(), componentUI);
    if (iter == mComponentUIs.end())
        mComponentUIs.push_back(componentUI);
}

void ServiceUIRender::Unregister(ComponentUI* componentUI)
{
    auto iter = std::find(mComponentUIs.begin(), mComponentUIs.end(), componentUI);
    if (iter != mComponentUIs.end())
        mComponentUIs.erase(iter);
}