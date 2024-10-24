#include "Precompiled.h"
#include "ComponentMesh.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
	ComponentRenderObject::Deserialize(value);

	Model::MeshData& mesh = mModel.meshData.emplace_back();
	Model::MaterialData& matData = mModel.materialData.emplace_back();
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			std::string shapeType = shapeData["Type"].GetString();
		}
		else
		{
			ASSERT(false, "MeshComponent: must specify a shape type");
		}
	}
	else
	{
		ASSERT(false, "MeshComponent: must have shape data");
	}

	if (value.HasMember("Material"))
	{

	}

	if (value.HasMember("Textures"))
	{

	}
}

const Model& CultyEngine::MeshComponent::GetModel() const
{
	return mModel;
}
