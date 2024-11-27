#include "ServiceCustomDebugDraw.h"
#include "ComponentCustomDebugDraw.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::MathC;

void ServiceCustomDebugDraw::Render()
{
    for (ComponentCustomDebugDraw* componentDebugDraw : mCustomDebugDrawComponents)
        componentDebugDraw->AddDebugDraw();
}

void ServiceCustomDebugDraw::Register(ComponentCustomDebugDraw* componentDebugDraw)
{
    auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), componentDebugDraw);
    if (iter == mCustomDebugDrawComponents.end())
        mCustomDebugDrawComponents.push_back(componentDebugDraw);
}

void ServiceCustomDebugDraw::Unregister(ComponentCustomDebugDraw* componentDebugDraw)
{
    auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), componentDebugDraw);
    if (iter != mCustomDebugDrawComponents.end())
        mCustomDebugDrawComponents.erase(iter);
}