#include "Precompiled.h"
#include "ComponentUIText.h"

#include "GameWorld.h"
#include "ServiceUIRender.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentUIText::Initialize()
{
    ServiceUIRender* renderService = GetOwner().GetWorld().GetService<ServiceUIRender>();
    renderService->Register(this);
}

void ComponentUIText::Terminate()
{
    ServiceUIRender* renderService = GetOwner().GetWorld().GetService<ServiceUIRender>();
    renderService->Unregister(this);
}

void ComponentUIText::Render()
{
    UIFont::Get()->DrawString(mText.wstring().c_str(), mPosition, mSize, mColor);
}

void ComponentUIText::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Text"))
        mText = value["Text"].GetString();

    if (value.HasMember("Size"))
        mSize = value["Size"].GetFloat();

    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
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