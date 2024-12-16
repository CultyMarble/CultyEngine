#include "Precompiled.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace CultyEngine;

static uint32_t gUniqueID = 0;

void GameObject::Initialize()
{
    ASSERT(!mInitialized, "GameObject: is already initialized!");

    for (auto& component : mComponents)
        component->Initialize();

    mUniqueID = ++gUniqueID;
    mInitialized = true;

    for (GameObject* child : mChildren)
        child->Initialize();
}

void GameObject::Terminate()
{
    for (auto& component : mComponents)
    {
        component->Terminate();
        component.reset();
    }
    mComponents.clear();

    for (GameObject* child : mChildren)
        child->Terminate();
    mChildren.clear();
    mParent = nullptr;
}

void GameObject::Update(float deltaTime)
{
    for (auto& component : mComponents)
        component->Update(deltaTime);
}

void GameObject::DebugUI()
{
    ImGui::PushID(mUniqueID);
    if (ImGui::CollapsingHeader(mName.c_str()))
    {
        for (auto& component : mComponents)
            component->DebugUI();

        if (!mTemplateFilePath.empty())
            if (ImGui::Button("Save"))
                Save();

        if (ImGui::Button("Delete"))
            mWorld->DestroyGameObject(mHandle);
    }
    ImGui::PopID();
}

void GameObject::SetName(std::string& name)
{
    mName = std::move(name);
}

const std::string& GameObject::GetName() const
{
    return mName;
}

uint32_t GameObject::GetUniqueID() const
{
    return mUniqueID;
}

void GameObject::Save()
{
    if (mTemplateFilePath.empty())
        return;

    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value components(rapidjson::kObjectType);

    for (auto& component : mComponents)
        component->Serialize(doc, components);

    doc.AddMember("Components", components, doc.GetAllocator());

    // parented objects
    FILE* file = nullptr;
    auto err = fopen_s(&file, mTemplateFilePath.u8string().c_str(), "w");
    ASSERT(err == 0, "GameObject: failed to open template file %s", mTemplateFilePath.u8string().c_str());

    char writeBuffer[65536];
    rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);

    writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
    doc.Accept(writer);
    fclose(file);
}