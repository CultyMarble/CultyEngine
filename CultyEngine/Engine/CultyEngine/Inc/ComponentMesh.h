#pragma once
#include "ComponentRenderObject.h"

namespace CultyEngine
{
    class ComponentMesh final : public ComponentRenderObject
    {
    public:
        SET_TYPE_ID(ComponentID::Mesh);

        void Deserialize(const rapidjson::Value& value) override;
        const Graphics::Model& GetModel() const override;

    private:
        Graphics::Model mModel;
    };
}