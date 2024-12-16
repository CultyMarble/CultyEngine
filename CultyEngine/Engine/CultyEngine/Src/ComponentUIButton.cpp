#include "Precompiled.h"
#include "ComponentUISprite.h"
#include "ComponentUIButton.h"

#include "GameWorld.h"
#include "ServiceUIRender.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

void ComponentUIButton::Initialize()
{
    for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
    {
        if (mButtonStateTextures[i].empty())
            continue;
        mButtonStates[i].Initialize(mButtonStateTextures[i]);
    }

    ServiceUIRender* serviceUIRender = GetOwner().GetWorld().GetService<ServiceUIRender>();
    serviceUIRender->Register(this);
}

void ComponentUIButton::Terminate()
{
    ServiceUIRender* serviceUIRender = GetOwner().GetWorld().GetService<ServiceUIRender>();
    serviceUIRender->Unregister(this);

    for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
        mButtonStates[i].Terminate();
}

void ComponentUIButton::Update(float deltaTime)
{
    mCurrentState = ButtonState::Default;

    InputSystem* inputSystem = InputSystem::Get();
    const int mouseX = inputSystem->GetMouseScreenX();
    const int mouseY = inputSystem->GetMouseScreenY();

    if (mButtonStates[0].IsInSprite(mouseX, mouseY))
    {
        mCurrentState = ButtonState::Hover;

        if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
            mCurrentState = ButtonState::Click;

        if (inputSystem->IsMousePressed(MouseButton::LBUTTON))
            OnClick();
    }
}

void ComponentUIButton::Render()
{
    uint32_t buttonState = static_cast<uint32_t>(mCurrentState);
    if (mButtonStateTextures[buttonState].empty())
        buttonState = 0;

    // Calculate Render Position
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

    mButtonStates[buttonState].SetPosition(worldPosition);
    UISpriteRenderer::Get()->Render(&mButtonStates[buttonState]);
}

void ComponentUIButton::Deserialize(const rapidjson::Value& value)
{
    const uint32_t buttonCount = static_cast<uint32_t>(ButtonState::Count);
    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }
    if (value.HasMember("Rotation"))
    {
        const float rotation = value["Rotation"].GetFloat();
        for (uint32_t i = 0; i < buttonCount; ++i)
            mButtonStates[i].SetRotation(rotation);
    }
    if (value.HasMember("Pivot"))
    {
        std::string pivot = value["Pivot"].GetString();
        Pivot buttonPivot = Pivot::TopLeft;
        if (pivot == "TopLeft")
        {
            buttonPivot = Pivot::TopLeft;
        }
        else if (pivot == "Top")
        {
            buttonPivot = Pivot::Top;
        }
        else if (pivot == "TopRight")
        {
            buttonPivot = Pivot::TopRight;
        }
        else if (pivot == "Left")
        {
            buttonPivot = Pivot::Left;
        }
        else if (pivot == "Center")
        {
            buttonPivot = Pivot::Center;
        }
        else if (pivot == "Right")
        {
            buttonPivot = Pivot::Right;
        }
        else if (pivot == "BottomLeft")
        {
            buttonPivot = Pivot::BottomLeft;
        }
        else if (pivot == "Bottom")
        {
            buttonPivot = Pivot::Bottom;
        }
        else if (pivot == "BottomRight")
        {
            buttonPivot = Pivot::BottomRight;
        }
        else
        {
            ASSERT(false, "ComponentUIButton: invalid pivot %s", pivot.c_str());
        }

        for (uint32_t i = 0; i < buttonCount; ++i)
            mButtonStates[i].SetPivot(buttonPivot);
    }

    for (uint32_t i = 0; i < buttonCount; ++i)
    {
        std::string buttonStateStr = "";
        ButtonState state = (ButtonState)i;
        switch (state)
        {
        case CultyEngine::ButtonState::Default: buttonStateStr = "Default"; break;
        case CultyEngine::ButtonState::Hover:   buttonStateStr = "Hover";   break;
        case CultyEngine::ButtonState::Click:   buttonStateStr = "Click";   break;
        case CultyEngine::ButtonState::Disable: buttonStateStr = "Disable"; break;
        default:
            break;
        }

        if (value.HasMember(buttonStateStr.c_str()) == false)
            continue;

        auto buttonStateObj = value[buttonStateStr.c_str()].GetObj();
        if (buttonStateObj.HasMember("Texture"))
        {
            mButtonStateTextures[i] = buttonStateObj["Texture"].GetString();
        }
        if (buttonStateObj.HasMember("Scale"))
        {
            auto scale = buttonStateObj["Scale"].GetArray();
            const float x = scale[0].GetFloat();
            const float y = scale[1].GetFloat();
            mButtonStates[i].SetScale({ x, y });
        }
        if (buttonStateObj.HasMember("Color"))
        {
            auto color = buttonStateObj["Color"].GetArray();
            const float r = color[0].GetFloat();
            const float g = color[1].GetFloat();
            const float b = color[2].GetFloat();
            const float a = color[3].GetFloat();
            mButtonStates[i].SetColor({ r, g, b, a });
        }
        if (buttonStateObj.HasMember("Flip"))
        {
            std::string flip = buttonStateObj["Flip"].GetString();
            if (flip == "None")
            {
                mButtonStates[i].SetFlip(Flip::None);
            }
            else if (flip == "Horizontal")
            {
                mButtonStates[i].SetFlip(Flip::Horizontal);
            }
            else if (flip == "Vertical")
            {
                mButtonStates[i].SetFlip(Flip::Vertical);
            }
            else if (flip == "Both")
            {
                mButtonStates[i].SetFlip(Flip::Both);
            }
            else
            {
                ASSERT(false, "ComponentUIButton: Invalid flip %s", flip.c_str());
            }
        }
    }
}

MathC::Vector2 ComponentUIButton::GetPosition(bool includeOrigin)
{
    float x = {};
    float y = {};

    if (includeOrigin)
        mButtonStates[0].GetOrigin(x, y);

    return { mPosition.x - x, mPosition.y - y };
}

void ComponentUIButton::SetCallback(ButtonCallback cb)
{
    mCallback = cb;
}

void ComponentUIButton::OnClick()
{
    LOG("BUTTON WAS CLICKED!!!");
    if (mCallback != nullptr)
        mCallback;
}