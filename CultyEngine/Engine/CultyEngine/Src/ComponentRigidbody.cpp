#include "Precompiled.h"
#include "ComponentRigidbody.h"
#include "ComponentTransform.h"

#include "GameWorld.h"
#include "ServicePhysics.h"

using namespace CultyEngine;
using namespace CultyEngine::Physics;

void ComponentRigidbody::Initialize()
{
    ServicePhysics* servicePhysics = GetOwner().GetWorld().GetService<ServicePhysics>();
    if (servicePhysics != nullptr)
    {
        ComponentTransform* componentTransform = GetOwner().GetComponent<ComponentTransform>();
        mRigidbody.Initialize(*componentTransform, mCollisionShape, mMass);
        servicePhysics->Register(this);
    }
}

void ComponentRigidbody::Terminate()
{
    ServicePhysics* servicePhysics = GetOwner().GetWorld().GetService<ServicePhysics>();
    if (servicePhysics != nullptr)
        servicePhysics->Unregister(this);

    mRigidbody.Terminate();
    mCollisionShape.Terminate();
}

void ComponentRigidbody::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Mass"))
    {
        mMass = value["Mass"].GetFloat();
    }

    if (value.HasMember("ColliderData"))
    {
        mCollisionShape.Terminate();
        auto colliderData = value["ColliderData"].GetObj();
        if (colliderData.HasMember("Shape"))
        {
            std::string shape = colliderData["Shape"].GetString();
            if (shape == "Empty")
            {
                mCollisionShape.InitializeEmpty();
            }
            else if (shape == "Box")
            {
                const auto halfExtents = colliderData["HalfExtents"].GetArray();
                const float x = halfExtents[0].GetFloat();
                const float y = halfExtents[1].GetFloat();
                const float z = halfExtents[2].GetFloat();
                mCollisionShape.InitializeBox({ x, y, z });
            }
            else if (shape == "Sphere")
            {
                const float radius = colliderData["Radius"].GetFloat();
                mCollisionShape.InitializeSphere(radius);
            }
            else if (shape == "Hull")
            {
                const auto halfExtents = colliderData["HalfExtents"].GetArray();
                const float hx = halfExtents[0].GetFloat();
                const float hy = halfExtents[1].GetFloat();
                const float hz = halfExtents[2].GetFloat();
                const auto origin = colliderData["Origin"].GetArray();
                const float ox = origin[0].GetFloat();
                const float oy = origin[1].GetFloat();
                const float oz = origin[2].GetFloat();
                mCollisionShape.InitializeHull({ hx, hy, hz }, { ox, oy, oz });
            }
            else
            {
                ASSERT(false, "ComponentRigidbody: invalid shape type %s", shape.c_str());
            }
        }
        else
        {
            ASSERT(false, "ComponentRigidbody: does not have shape data.");
        }
    }
}

void ComponentRigidbody::SetPosition(const MathC::Vector3& position)
{
    mRigidbody.SetPosition(position);
}

void ComponentRigidbody::SetVelocity(const MathC::Vector3& velocity)
{
    mRigidbody.SetVelocity(velocity);
}