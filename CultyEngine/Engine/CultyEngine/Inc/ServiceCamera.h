#pragma once
#include "Service.h"

namespace CultyEngine
{
    class ComponentCamera;

    class ServiceCamera final : public Service
    {
    public:
        SET_TYPE_ID(ServiceID::Camera);

        void DebugUI() override;

        const Graphics::Camera& GetMain() const;
        void SetMainCamera(uint32_t index);

        void Register(const ComponentCamera* cameraComponent);
        void Unregister(const ComponentCamera* cameraComponent);

    private:
        using CameraEntries = std::vector<const ComponentCamera*>;
        CameraEntries mCameraEntries;
        const ComponentCamera* mMainCamera = nullptr;
    };
}