#pragma once

#include "PhysicsObject.h"

namespace CultyEngine::Physics
{
    class CollisionShape;

    class RigidBody final : public PhysicsObject
    {
    public:
        RigidBody() = default;
        ~RigidBody() override;

        void Initialize(CultyEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
        void Terminate();

        void SetPosition(const CultyEngine::MathC::Vector3& position);
        void SetVelocity(const CultyEngine::MathC::Vector3& velocity);

        bool IsDynamic() const;

    private:
        void SyncGraphics() override;
        btRigidBody* GetRigidBody() override { return mRigidBody; }

        btRigidBody* mRigidBody = nullptr;
        btDefaultMotionState* mMotionState = nullptr;
        float mMass = 0.0f;

        CultyEngine::Graphics::Transform* mGraphicsTransform = nullptr;
    };
}