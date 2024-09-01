#pragma once

#include "RigidBody.h"
#include "CollisionShape.h"

namespace CultyEngine::Physics
{
    struct ParticleActivateData
    {
        float lifeTime = 0.0f;
        Color startColor = Colors::White;
        Color endColor = Colors::White;
        MathC::Vector3 startScale = MathC::Vector3::One;
        MathC::Vector3 endScale = MathC::Vector3::One;
        MathC::Vector3 position = MathC::Vector3::One;
        MathC::Vector3 velocity = MathC::Vector3::One;
    };

    struct CurrentParticleInfo
    {
        Color color = Colors::White;
        Graphics::Transform transform;
    };

    class Particle
    {
    public:
        void Initialize();
        void Terminate();
        void Activate(const ParticleActivateData& data);
        void Update(float deltaTime);

        bool IsActive() const;
        void ObtainCurrentInfo(CurrentParticleInfo& info) const;
        const MathC::Vector3& GetPosition() const;

    private:
        Graphics::Transform mTransform;
        RigidBody mRigidBody;
        CollisionShape mCollisionShape;

        ParticleActivateData mData;
        float mLifeTime = 0.0f;
    };
}
