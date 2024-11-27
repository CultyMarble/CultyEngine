#pragma once
#include "Service.h"

namespace CultyEngine
{
    class ComponentRigidbody;

    class ServicePhysics final : public Service
    {
    public:
        SET_TYPE_ID(ServiceID::Physics);

        void Update(float deltaTime) override;
        void DebugUI() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        void Deserialize(const rapidjson::Value& value) override;

        void Register(ComponentRigidbody* componentRigidbody);
        void Unregister(ComponentRigidbody* componentRigidbody);

        void SetEnabled(bool enable);

    private:
        bool mEnabled = true;
    };
}