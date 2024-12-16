#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentAnimator.h"
#include "ComponentCamera.h"
#include "ComponentCameraFPS.h"
#include "ComponentMesh.h"
#include "ComponentModel.h"
#include "ComponentRigidbody.h"
#include "ComponentSoundBank.h"
#include "ComponentSoundEffect.h"
#include "ComponentTransform.h"
#include "ComponentUIButton.h"
#include "ComponentUISprite.h"
#include "ComponentUIText.h"

#include "GameWorld.h"

using namespace CultyEngine;

namespace
{
    CustomMake TryMake;
    CustomMake TryGet;

    Component* AddComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;

        if (componentName == "ComponentAnimator")
            newComponent = gameObject.AddComponent<ComponentAnimator>();

        else if (componentName == "ComponentCamera")
            newComponent = gameObject.AddComponent<ComponentCamera>();

        else if (componentName == "ComponentCameraFPS")
            newComponent = gameObject.AddComponent<ComponentCameraFPS>();

        else if (componentName == "ComponentMesh")
            newComponent = gameObject.AddComponent<ComponentMesh>();

        else if (componentName == "ComponentModel")
            newComponent = gameObject.AddComponent<ComponentModel>();

        else if (componentName == "ComponentRigidbody")
            newComponent = gameObject.AddComponent<ComponentRigidbody>();

        else if (componentName == "ComponentSoundBank")
            newComponent = gameObject.AddComponent<ComponentSoundBank>();

        else if (componentName == "ComponentSoundEffect")
            newComponent = gameObject.AddComponent<ComponentSoundEffect>();

        else if (componentName == "ComponentTransform")
            newComponent = gameObject.AddComponent<ComponentTransform>();

        else if (componentName == "ComponentUIButton")
            newComponent = gameObject.AddComponent<ComponentUIButton>();

        else if (componentName == "ComponentUISprite")
            newComponent = gameObject.AddComponent<ComponentUISprite>();

        else if (componentName == "ComponentUIText")
            newComponent = gameObject.AddComponent<ComponentUIText>();

        else
        {
            newComponent = TryMake(componentName, gameObject);
            ASSERT(newComponent != nullptr, "GameObjectFactory: unrecognized component %s", componentName.c_str());
        }

        return newComponent;
    }

    Component* GetComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;

        if (componentName == "ComponentAnimator")
            newComponent = gameObject.GetComponent<ComponentAnimator>();

        else if (componentName == "ComponentCamera")
            newComponent = gameObject.GetComponent<ComponentCamera>();

        else if (componentName == "ComponentCameraFPS")
            newComponent = gameObject.GetComponent<ComponentCameraFPS>();

        else if (componentName == "ComponentMesh")
            newComponent = gameObject.GetComponent<ComponentMesh>();

        else if (componentName == "ComponentModel")
            newComponent = gameObject.GetComponent<ComponentModel>();

        else if (componentName == "ComponentRigidbody")
            newComponent = gameObject.GetComponent<ComponentRigidbody>();

        else if (componentName == "ComponentSoundBank")
            newComponent = gameObject.GetComponent<ComponentSoundBank>();

        else if (componentName == "ComponentSoundEffect")
            newComponent = gameObject.GetComponent<ComponentSoundEffect>();

        else if (componentName == "ComponentTransform")
            newComponent = gameObject.GetComponent<ComponentTransform>();

        else if (componentName == "ComponentUIButton")
            newComponent = gameObject.GetComponent<ComponentUIButton>();

        else if (componentName == "ComponentUISprite")
            newComponent = gameObject.GetComponent<ComponentUISprite>();

        else if (componentName == "ComponentUIText")
            newComponent = gameObject.GetComponent<ComponentUIText>();

        else
        {
            newComponent = TryGet(componentName, gameObject);
            ASSERT(newComponent != nullptr, "GameObjectFactory: unrecognized component %s", componentName.c_str());
        }
        return newComponent;
    }
}

void GameObjectFactory::SetCustomMake(CustomMake customMake)
{
    TryMake = customMake;
}

void GameObjectFactory::SetCustomGet(CustomGet customGet)
{
    TryGet = customGet;
}

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& gameWorld)
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

    // Fix child and parent initialize order
    if (doc.HasMember("Children"))
    {
        auto children = doc["Children"].GetObj();
        for (auto& child : children)
        {
            std::string name = child.name.GetString();
            std::filesystem::path childTemplate = child.value["Template"].GetString();
            GameObject* go = gameWorld.CreateGameObject(name, childTemplate);
            GameObjectFactory::OverrideDeserialize(child.value, *go);

            gameObject.AddChild(go);
            go->SetParent(&gameObject);
        }
    }
}

void GameObjectFactory::OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject)
{
    if (value.HasMember("Components") == false)
        return;

    auto components = value["Components"].GetObj();
    for (auto& component : components)
    {
        Component* ownedComponent = GetComponent(component.name.GetString(), gameObject);

        if (ownedComponent != nullptr)
            ownedComponent->Deserialize(component.value);
    }
}