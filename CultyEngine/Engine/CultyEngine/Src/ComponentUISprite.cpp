#include "Precompiled.h"
#include "ComponentUISprite.h"

#include "GameWorld.h"
#include "ServiceUIRender.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentUISprite::Initialize()
{
    ASSERT(!mTexturePath.empty(), "ComponentUISprite: texture path is not set");
    mUISprite.Initialize(mTexturePath);
    ServiceUIRender* renderService = GetOwner().GetWorld().GetService<ServiceUIRender>();
    renderService->Register(this);
}

void ComponentUISprite::Terminate()
{
    ServiceUIRender* renderService = GetOwner().GetWorld().GetService<ServiceUIRender>();
    renderService->Unregister(this);
    mUISprite.Terminate();
}

void ComponentUISprite::Render()
{
    UISpriteRenderer::Get()->Render(&mUISprite);
}

void ComponentUISprite::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Texture"))
        mTexturePath = value["Texture"].GetString();

    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        const float x = pos[0].GetFloat();
        const float y = pos[1].GetFloat();
        mUISprite.SetPosition({ x, y });
    }

    if (value.HasMember("Scale"))
    {
        auto scale = value["Position"].GetArray();
        const float x = scale[0].GetFloat();
        const float y = scale[1].GetFloat();
        mUISprite.SetScale({ x, y });
    }

    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        const float r = color[0].GetFloat();
        const float g = color[1].GetFloat();
        const float b = color[2].GetFloat();
        const float a = color[3].GetFloat();
        mUISprite.SetColor({ r, g, b, a });
    }

    if (value.HasMember("Rotation"))
    {
        auto rotation = value["Rotation"].GetFloat();
        mUISprite.SetRotation(rotation);
    }

    if (value.HasMember("Pivot"))
    {
        std::string pivot = value["Pivot"].GetString();
        if (pivot == "TopLeft")
        {
            mUISprite.SetPivot(Pivot::TopLeft);
        }
        else if (pivot == "Top")
        {
            mUISprite.SetPivot(Pivot::Top);
        }
        else if (pivot == "TopRight")
        {
            mUISprite.SetPivot(Pivot::TopRight);
        }
        else if (pivot == "Left")
        {
            mUISprite.SetPivot(Pivot::Left);
        }
        else if (pivot == "Center")
        {
            mUISprite.SetPivot(Pivot::Center);
        }
        else if (pivot == "Right")
        {
            mUISprite.SetPivot(Pivot::Right);
        }
        else if (pivot == "BottomLeft")
        {
            mUISprite.SetPivot(Pivot::BottomLeft);
        }
        else if (pivot == "Bottom")
        {
            mUISprite.SetPivot(Pivot::Bottom);
        }
        else if (pivot == "BottomRight")
        {
            mUISprite.SetPivot(Pivot::BottomRight);
        }
        else
        {
            ASSERT(false, "ComponentUISprite: invalid pivot %s", pivot.c_str());
        }
    }

    if (value.HasMember("Flip"))
    {
        std::string flip = value["Flip"].GetString();
        if (flip == "None")
        {
            mUISprite.SetFlip(Flip::None);
        }
        else if (flip == "Horizontal")
        {
            mUISprite.SetFlip(Flip::Horizontal);
        }
        else if (flip == "Vertical")
        {
            mUISprite.SetFlip(Flip::Vertical);
        }
        else if (flip == "Both")
        {
            mUISprite.SetFlip(Flip::Both);
        }
        else
        {
            ASSERT(false, "ComponentUISprite: invalid flip %s", flip.c_str());
        }
    }
}