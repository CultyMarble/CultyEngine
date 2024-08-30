#pragma once

namespace CultyEngine::Physics
{
    class CollisionShape final
    {
    public:
        CollisionShape() = default;
        ~CollisionShape();

        void InitializeSphere(float radius);
        void InitializeBox(const CultyEngine::MathC::Vector3& halfExtents);
        void InitializeHull(const CultyEngine::MathC::Vector3& halfExtents, const CultyEngine::MathC::Vector3& origin);
        void InitializeEmpty();
        void Terminate();

    private:
        btCollisionShape* mCollisionShape = nullptr;
        
        friend class RigidBody;
        btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
    };
}