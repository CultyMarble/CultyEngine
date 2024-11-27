#include "Precompiled.h"
#include "ServicePhysics.h"
#include "SaveUtil.h"
#include "ComponentRigidbody.h"

using namespace CultyEngine;
using namespace CultyEngine::Physics;

void ServicePhysics::Update(float deltaTime)
{
    if (mEnabled)
        PhysicsWorld::Get()->Update(deltaTime);
}

void ServicePhysics::DebugUI()
{
    if (mEnabled)
        PhysicsWorld::Get()->DebugUI();
}

void ServicePhysics::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    PhysicsWorld::Settings settings = PhysicsWorld::Get()->GetSettings();
    rapidjson::Value serviceValue(rapidjson::kObjectType);
    SaveUtil::SaveVector3("Gravity", settings.gravity, doc, serviceValue);
    SaveUtil::SaveFloat("SimSteps", settings.simulationSteps, doc, serviceValue);
    SaveUtil::SaveFloat("FixedTimeStep", settings.fixedTimeStep, doc, serviceValue);
    value.AddMember("ServicePhysics", serviceValue, doc.GetAllocator());
}

void ServicePhysics::Deserialize(const rapidjson::Value& value)
{
    PhysicsWorld::Settings settings = PhysicsWorld::Get()->GetSettings();
    if (value.HasMember("Gravity"))
    {
        const auto& gravity = value["Gravity"].GetArray();
        settings.gravity.x = gravity[0].GetFloat();
        settings.gravity.y = gravity[1].GetFloat();
        settings.gravity.z = gravity[2].GetFloat();
    }
    if (value.HasMember("SimSteps"))
    {
        settings.simulationSteps = value["SimSteps"].GetFloat();
    }
    if (value.HasMember("FixedTimeStep"))
    {
        settings.fixedTimeStep = value["FixedTimeStep"].GetFloat();
    }
    PhysicsWorld::Get()->UpdateSettings(settings);
}

void ServicePhysics::Register(ComponentRigidbody* componentRigidbody)
{
    PhysicsWorld::Get()->Register(&componentRigidbody->mRigidbody);
}

void ServicePhysics::Unregister(ComponentRigidbody* componentRigidbody)
{
    PhysicsWorld::Get()->Unregister(&componentRigidbody->mRigidbody);
}

void ServicePhysics::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}