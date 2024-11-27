#pragma once
#include "Service.h"

namespace CultyEngine
{
    class ComponentUI;

    class ServiceUIRender final : public Service
    {
    public:
        SET_TYPE_ID(ServiceID::UIRender);

        void Initialize() override;
        void Terminate() override;

        void Update(float deltaTime) override;
        void Render() override;

        void Register(ComponentUI* componentUI);
        void Unregister(ComponentUI* componentUI);

    private:
        using ComponentUIs = std::vector<ComponentUI*>;
        ComponentUIs mComponentUIs;
    };
}