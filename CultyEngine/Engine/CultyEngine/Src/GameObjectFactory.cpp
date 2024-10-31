#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentCameraFPS.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

using namespace CultyEngine;

namespace
{
    Component* AddComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;
        if (componentName == "ComponentCamera")
        {
            newComponent = gameObject.AddComponent<ComponentCamera>();
        }
        else if (componentName == "ComponentCameraFPS")
        {
            newComponent = gameObject.AddComponent<ComponentCameraFPS>();
        }
        else if (componentName == "ComponentMesh")
        {
            newComponent = gameObject.AddComponent<ComponentMesh>();
        }
        else if (componentName == "ComponentTransform")
        {
            newComponent = gameObject.AddComponent<ComponentTransform>();
        }
        else
        {
            ASSERT(false, "GameObjectFactory: unrecognized component %s", componentName.c_str());
        }
        return newComponent;
    }

    Component* GetComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;
        if (componentName == "ComponentCamera")
        {
            newComponent = gameObject.GetComponent<ComponentCamera>();
        }
        else if (componentName == "ComponentCameraFPS")
        {
            newComponent = gameObject.GetComponent<ComponentCameraFPS>();
        }
        else if (componentName == "ComponentMesh")
        {
            newComponent = gameObject.GetComponent<ComponentMesh>();
        }
        else if (componentName == "ComponentTransform")
        {
            newComponent = gameObject.GetComponent<ComponentTransform>();
        }
        else
        {
            ASSERT(false, "GameObjectFactory: unrecognized component %s", componentName.c_str());
        }
        return newComponent;
    }
}

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");
    ASSERT(err == 0, "GameObjectFactory: failed to open file %s", templatePath.u8string().c_str());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);
    auto components = doc["Components"].GetObj();
    for (auto& component : components)
    {
        Component* newComponent = AddComponent(component.name.GetString(), gameObject);
        if (newComponent != nullptr)
        {
            newComponent->Deserialize(component.value);
        }
    }
}

void GameObjectFactory::OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject)
{
    if (value.HasMember("Component"))
    {
        auto components = value["Components"].GetObj();
        for (auto& component : components)
        {
            Component* ownedComponent = GetComponent(component.name.GetString(), gameObject);
            if (ownedComponent != nullptr)
            {
                ownedComponent->Deserialize(component.value);
            }
        }
    }
}
