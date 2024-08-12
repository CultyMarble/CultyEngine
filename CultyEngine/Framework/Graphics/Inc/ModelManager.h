#pragma once
#include "Model.h"

namespace CultyEngine::Graphics
{
    using ModelID = std::size_t;

    class ModelManager final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static ModelManager* Get();

        ModelManager() = default;
        ~ModelManager() = default;

        ModelManager(const ModelManager&) = delete;
        ModelManager(const ModelManager&&) = delete;
        ModelManager& operator=(const ModelManager&) = delete;
        ModelManager& operator=(const ModelManager&&) = delete;

        ModelID GetModelID(const std::filesystem::path& filePath);
        ModelID LoadModelID(const std::filesystem::path& filePath);
        void AddAnimation(ModelID id, const std::filesystem::path& filePath);
        const Model* GetModel(ModelID id) const;

    private:
        using ModelCache = std::map<ModelID, std::unique_ptr<Model>>;
        ModelCache mInventory;
    };
}