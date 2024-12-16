#pragma once
#include "Component.h"
#include "GameObjectHandle.h"

namespace CultyEngine
{
    class GameWorld;

    class GameObject final
    {
    public:
        GameObject() = default;

        void Initialize();
        void Terminate();
        void Update(float deltaTime);
        void DebugUI();

        void SetName(std::string& name);
        const std::string& GetName() const;
        uint32_t GetUniqueID() const;

        GameWorld& GetWorld() { return *mWorld; }
        const GameWorld& GetWorld() const { return *mWorld; }
        const GameObjectHandle& GetHandle() { return mHandle; }

        void SetParent(GameObject* parent) { mParent = parent; }
        GameObject* GetParent() { return mParent; }
        const GameObject* GetParent() const { return mParent; }

        void AddChild(GameObject* child) { mChildren.push_back(child); }
        GameObject* GetChild(uint32_t index) { return mChildren[index]; }
        const GameObject* GetChild(uint32_t index) const { return mChildren[index]; }

        template<class ComponentType>
        ComponentType* AddComponent()
        {
            static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject: ComponentType must be of type Component!");

            ASSERT(mInitialized == false, "GameObject: cannot add components once initialized");
            ASSERT(ComponentType::StaticGetTypeID() != static_cast<uint32_t>(ComponentID::Invalid), "GameObject: Invalid component type ID");
            ASSERT(HasA<ComponentType>() == false, "GameObject: already has component type");

            auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
            newComponent->mOwner = this;
            return static_cast<ComponentType*>(newComponent.get());
        }

        template<class ComponentType>
        bool HasA()
        {
            static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject: ComponentType must be of type Component");
            for (auto& component : mComponents)
            {
                if (component->GetTypeID() == ComponentType::StaticGetTypeID())
                    return true;
            }
            return false;
        }

        template<class ComponentType>
        const ComponentType* GetComponent() const
        {
            static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject: ComponentType must be of type Component");
            for (auto& component : mComponents)
            {
                if (component->GetTypeID() == ComponentType::StaticGetTypeID())
                    return static_cast<ComponentType*>(component.get());
            }
            return nullptr;
        }

        template<class ComponentType>
        ComponentType* GetComponent()
        {
            const GameObject* thisConst = static_cast<const GameObject*>(this);
            return const_cast<ComponentType*>(thisConst->GetComponent<ComponentType>());
        }

    private:
        void Save();

        std::string mName = "EMPTY";
        bool mInitialized = false;
        uint32_t mUniqueID = 0;

        using Components = std::vector<std::unique_ptr<Component>>;
        Components mComponents;

        using Children = std::vector<GameObject*>;
        Children mChildren;

        friend class GameWorld;
        std::filesystem::path mTemplateFilePath = "";
        GameObjectHandle mHandle;
        GameWorld* mWorld = nullptr;
        GameObject* mParent = nullptr;
    };
}