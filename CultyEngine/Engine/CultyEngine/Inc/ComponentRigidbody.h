#pragma once
#include "Component.h"

namespace CultyEngine
{
    class ComponentRigidbody final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::Rigidbody);

        void Initialize() override;
        void Terminate() override;
        void Deserialize(const rapidjson::Value& value) override;

        void SetPosition(const MathC::Vector3& position);
        void SetVelocity(const MathC::Vector3& velocity);

    private:
        friend class ServicePhysics;

        Physics::CollisionShape mCollisionShape;
        Physics::RigidBody mRigidbody;
        float mMass = -1.0f;
    };
}