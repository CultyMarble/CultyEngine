#include "ComponentCustomDebugDraw.h"
#include "ServiceCustomDebugDraw.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::MathC;

void ComponentCustomDebugDraw::Initialize()
{
    mComponentTransform = GetOwner().GetComponent<ComponentTransform>();
    ServiceCustomDebugDraw* serviceDraw = GetOwner().GetWorld().GetService<ServiceCustomDebugDraw>();
    serviceDraw->Register(this);
}

void ComponentCustomDebugDraw::Terminate()
{
    ServiceCustomDebugDraw* serviceDraw = GetOwner().GetWorld().GetService<ServiceCustomDebugDraw>();
    serviceDraw->Unregister(this);
}

void ComponentCustomDebugDraw::Update(float deltaTime)
{

}

void ComponentCustomDebugDraw::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Slices"))
        mSlices = static_cast<uint32_t>(value["Slices"].GetInt());

    if (value.HasMember("Rings"))
        mRings = static_cast<uint32_t>(value["Rings"].GetInt());

    if (value.HasMember("Radius"))
        mRadius = value["Radius"].GetFloat();

    if (value.HasMember("Position"))
    {
        auto position = value["Position"].GetArray();
        mPosition.x = position[0].GetFloat();
        mPosition.y = position[1].GetFloat();
        mPosition.z = position[2].GetFloat();
    }

    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        mColor.r = color[0].GetFloat();
        mColor.g = color[1].GetFloat();
        mColor.b = color[2].GetFloat();
        mColor.a = color[3].GetFloat();
    }
}

void ComponentCustomDebugDraw::AddDebugDraw()
{
    Vector3 worldSpace = mPosition;
    if (mComponentTransform != nullptr)
    {
        Matrix4 matWorld = mComponentTransform->GetMatrix4();
        worldSpace = TransformCoord(mPosition, matWorld);
    }
    SimpleDraw::AddSphere(mSlices, mRings, mRadius, worldSpace, mColor);
}