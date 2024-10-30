#include "Precompiled.h"
#include "ComponentTransform.h"
#include "SaveUtil.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentTransform::DebugUI()
{
    ImGui::DragFloat3("Position", &position.x, 0.01f, -100.0f, 100.0f);
    ImGui::DragFloat4("Rotation", &rotation.x, 0.001f);
    ImGui::DragFloat3("Scale", &scale.x, 0.01f);

    SimpleDraw::AddTransform(GetMatrix4());
}

void ComponentTransform::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    SaveUtil::SaveVector3("Position", position, doc, componentValue);
    SaveUtil::SaveQuaternion("Rotation", rotation, doc, componentValue);
    SaveUtil::SaveVector3("Scale", scale, doc, componentValue);
    value.AddMember("ComponentTransform", componentValue, doc.GetAllocator());
}

void ComponentTransform::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Position"))
    {
        const auto& pos = value["Position"].GetArray();
        position.x = pos[0].GetFloat();
        position.y = pos[1].GetFloat();
        position.z = pos[2].GetFloat();
    }

    if (value.HasMember("Rotation"))
    {
        const auto& rot = value["Rotation"].GetArray();
        rotation.x = rot[0].GetFloat();
        rotation.y = rot[1].GetFloat();
        rotation.z = rot[2].GetFloat();
        rotation.w = rot[3].GetFloat();
    }

    if (value.HasMember("Scale"))
    {
        const auto& s = value["Scale"].GetArray();
        scale.x = s[0].GetFloat();
        scale.y = s[1].GetFloat();
        scale.z = s[2].GetFloat();
    }
}