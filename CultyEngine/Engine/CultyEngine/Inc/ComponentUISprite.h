#pragma once
#include "ComponentUI.h"

namespace CultyEngine
{
    class ComponentUISprite : public ComponentUI
    {
    public:
        SET_TYPE_ID(ComponentID::UISprite);

        void Initialize() override;
        void Terminate() override;
        void Render() override;
        void Deserialize(const rapidjson::Value& value) override;

    private:
        std::filesystem::path mTexturePath;
        Graphics::UISprite mUISprite;
    };
}