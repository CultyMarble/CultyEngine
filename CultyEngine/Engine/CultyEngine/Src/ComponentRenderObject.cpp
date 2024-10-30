#include "Precompiled.h"
#include "ComponentRenderObject.h"

#include "ServiceRender.h"
#include "GameWorld.h"

using namespace CultyEngine;

void ComponentRenderObject::Initialize()
{
    ServiceRender* renderService = GetOwner().GetWorld().GetService<ServiceRender>();
    renderService->Register(this);
}

void ComponentRenderObject::Terminate()
{
    ServiceRender* renderService = GetOwner().GetWorld().GetService<ServiceRender>();
    renderService->Unregister(this);
}

void ComponentRenderObject::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("CastShadow"))
        mCastShadow = value["CastShadow"].GetBool();
}