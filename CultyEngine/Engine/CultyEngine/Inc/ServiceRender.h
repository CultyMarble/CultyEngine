#pragma once
#include "Service.h"

namespace CultyEngine
{
    class ServiceCamera;
    class ComponentRenderObject;
    class ComponentTransform;

    class ServiceRender final : public Service
    {
    public:
        SET_TYPE_ID(ServiceID::Render);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void Render() override;
        void DebugUI() override;

        void Register(const ComponentRenderObject* componentRenderObject);
        void Unregister(const ComponentRenderObject* componentRenderObject);

    private:
        const ServiceCamera* mCameraService = nullptr;

        Graphics::DirectionalLight mDirectionalLight;
        Graphics::StandardEffect mStandardEffect;
        Graphics::ShadowEffect mShadowEffect;

        struct Entry
        {

            const ComponentRenderObject* componentRender = nullptr;
            const ComponentTransform* componentTransform = nullptr;
            Graphics::RenderGroup renderGroup;
        };

        using RenderEntries = std::vector<Entry>;
        RenderEntries mRenderEntries;

        float mFPS = 0.0f;
    };
}