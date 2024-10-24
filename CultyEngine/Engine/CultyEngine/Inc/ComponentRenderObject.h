#pragma once
#include "Component.h"

namespace CultyEngine
{
	class ComponentRenderObject : public Component
	{
	public:
		void Initialize() override;
		void Terminate() override;

		virtual void Deserialize(const rapidjson::Value& value) override;

		bool CanCastShadow() const { return mCastShadow; }
		virtual Graphics::ModelID GetModelID() const { return 0; }
		virtual const Graphics::Model& GetModel() const = 0;

	private:
		bool mCastShadow;
	};
}