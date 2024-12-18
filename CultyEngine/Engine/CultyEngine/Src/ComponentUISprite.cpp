#include "Precompiled.h"
#include "ComponentUISprite.h"
#include "ComponentUIButton.h"

#include "GameWorld.h"
#include "ServiceUIRender.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentUISprite::Initialize()
{
    //ASSERT(!mTexturePath.empty(), "ComponentUISprite: texture path is not set");
    if (!mTexturePath.empty())
        mUISprite.Initialize(mTexturePath);

    if (mRect.right - mRect.left > 0)
        mUISprite.SetRect(mRect.top, mRect.left, mRect.right, mRect.bottom);

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
    MathC::Vector2 worldPosition = GetPosition(false);
    GameObject* parent = GetOwner().GetParent();

    while (parent != nullptr)
    {
        ComponentUISprite* componentUISprite = parent->GetComponent<ComponentUISprite>();
        if (componentUISprite != nullptr)
        {
            worldPosition += componentUISprite->GetPosition();
        }
        else
        {
            ComponentUIButton* componentUIButton = parent->GetComponent<ComponentUIButton>();
            if (componentUIButton != nullptr)
            {
                worldPosition += componentUIButton->GetPosition();
            }
        }
        parent = parent->GetParent();
    }

    mUISprite.SetPosition({ worldPosition.x, worldPosition.y });

    if (mUISprite.mTexture != nullptr)
    {
        // Directly render the texture
        UISpriteRenderer::Get()->Render(mUISprite.mTexture, mUISprite);
    }
    else
    {
        // Fallback to default rendering
        UISpriteRenderer::Get()->Render(&mUISprite);
    }
}

void ComponentUISprite::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Texture"))
        mTexturePath = value["Texture"].GetString();

    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }

    if (value.HasMember("Scale"))
    {
        auto scale = value["Scale"].GetArray();
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
        mRotation = value["Rotation"].GetFloat();
        mUISprite.SetRotation(mRotation);
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

    if (value.HasMember("Rect"))
    {
        auto rect = value["Rect"].GetArray();
        mRect.top = rect[0].GetInt();
        mRect.left = rect[1].GetInt();
        mRect.right = rect[2].GetInt();
        mRect.bottom = rect[3].GetInt();
    }
}

MathC::Vector2 ComponentUISprite::GetPosition(bool includeOrigin)
{
    float x = {};
    float y = {};

    if (includeOrigin)
        mUISprite.GetOrigin(x, y);

    return { mPosition.x - x, mPosition.y - y };
}

void ComponentUISprite::SetTexture(Graphics::Texture* texture)
{
    if (texture != nullptr)
    {
        mUISprite.mTexture = texture;

        // Set mRect to cover the entire texture
        mUISprite.mRect = { 0, 0, static_cast<int>(texture->GetWidth()), static_cast<int>(texture->GetHeight()) };

        // Scale the texture to fill the screen (example for a 1280x720 screen)
        float screenWidth = 1280.0f; // Replace with actual screen width
        float screenHeight = 720.0f; // Replace with actual screen height
        mUISprite.mScale.x = screenWidth / texture->GetWidth();
        mUISprite.mScale.y = screenHeight / texture->GetHeight();

        // Center the sprite on the screen
        mUISprite.mPosition = { 0.0f, 0.0f };
    }
}