#include "Precompiled.h"
#include "ComponentCamera.h"

#include "GameWorld.h"
#include "ServiceCamera.h"

#include "SaveUtil.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentCamera::Initialize()
{
    ServiceCamera* cameraService = GetOwner().GetWorld().GetService<ServiceCamera>();
    cameraService->Register(this);
}

void ComponentCamera::Terminate()
{
    ServiceCamera* cameraService = GetOwner().GetWorld().GetService<ServiceCamera>();
    cameraService->Unregister(this);
}

void ComponentCamera::DebugUI()
{
    Vector3 pos = mCamera.GetPosition();
    if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
        mCamera.SetPosition(pos);

    Matrix4 matTrans = Matrix4::Translation(mCamera.GetPosition());
    SimpleDraw::AddTransform(matTrans);
}

void ComponentCamera::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    SaveUtil::SaveVector3("Position", mCamera.GetPosition(), doc, componentValue);
    SaveUtil::SaveVector3("LookAt", mCamera.GetPosition() + mCamera.GetDirection(), doc, componentValue);
    value.AddMember("CameraComponent", componentValue, doc.GetAllocator());
}

void ComponentCamera::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Position"))
    {
        const auto& pos = value["Position"].GetArray();
        float x = pos[0].GetFloat();
        float y = pos[1].GetFloat();
        float z = pos[2].GetFloat();
        mCamera.SetPosition({ x,y,z });
    }

    if (value.HasMember("LookAt"))
    {
        const auto& pos = value["LookAt"].GetArray();
        float x = pos[0].GetFloat();
        float y = pos[1].GetFloat();
        float z = pos[2].GetFloat();
        mCamera.SetLookAt({ x,y,z });
    }
}

Camera& ComponentCamera::GetCamera()
{
    return mCamera;
}

const Camera& ComponentCamera::GetCamera() const
{
    return mCamera;
}