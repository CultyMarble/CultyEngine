#pragma once
#include "ComponentRenderObject.h"

namespace CultyEngine
{
	class ComponentModel final : public ComponentRenderObject
	{
	public:
		SET_TYPE_ID(ComponentID::Model);

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value);
		void Deserialize(const rapidjson::Value& value) override;

		Graphics::ModelID GetModelID() const override;
		const Graphics::Model& GetModel() const override;

	private:
		std::string mFileName;
		Graphics::ModelID mModelID;

		using Animations = std::vector<std::string>;
	};
}