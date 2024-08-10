#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    std::unique_ptr<ModelManager> sModelManager;
}

void ModelManager::StaticInitialize()
{
    ASSERT(sModelManager == nullptr, "ModelManager: is already initialized!");
    sModelManager = std::make_unique<ModelManager>();
}

void ModelManager::StaticTerminate()
{
    sModelManager.reset();
}

ModelManager* ModelManager::Get()
{
    ASSERT(sModelManager != nullptr, "ModelManager: is not initialized!");
    return sModelManager.get();
}

ModelID ModelManager::GetModelID(const std::filesystem::path& filePath)
{
    return std::filesystem::hash_value(filePath);
}

ModelID ModelManager::LoadModelID(const std::filesystem::path& filePath)
{
    const ModelID modelID = GetModelID(filePath);
    auto [iter, success] = mInventory.insert({ modelID, nullptr });
    if (success)
    {
        auto& modelPtr = iter->second;
        modelPtr = std::make_unique<Model>();
        ModelIO::LoadModel(filePath, *modelPtr);
        ModelIO::LoadMaterial(filePath, *modelPtr);
        ModelIO::LoadSkeleton(filePath, *modelPtr);
    }
    return modelID;
}

const Model* ModelManager::GetModel(ModelID id) const
{
    auto model = mInventory.find(id);
    if (model != mInventory.end())
    {
        return model->second.get();
    }
    return nullptr;
}