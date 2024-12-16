#pragma once
#include "ComponentUI.h"

namespace CultyEngine
{
    enum class ButtonState
    {
        Default,
        Hover,
        Click,
        Disable,
        Count,
    };

    using ButtonCallback = std::function<void()>;

    class ComponentUIButton : public ComponentUI
    {
    public:
        SET_TYPE_ID(ComponentID::UIButton);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Deserialize(const rapidjson::Value& value) override;
        MathC::Vector2 GetPosition(bool includeOrigin = true);

        void SetCallback(ButtonCallback cb);

    private:
        void OnClick();

        ButtonCallback mCallback = nullptr;
        DirectX::XMFLOAT2 mPosition = { 0.0f, 0.0f };
        Graphics::UISprite mButtonStates[static_cast<uint32_t>(ButtonState::Count)];
        std::string mButtonStateTextures[static_cast<uint32_t>(ButtonState::Count)];
        ButtonState mCurrentState = ButtonState::Default;
    };
}