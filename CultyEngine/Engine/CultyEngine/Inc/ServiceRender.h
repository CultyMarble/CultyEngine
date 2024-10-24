#pragma once
#include "Service.h"

namespace CultyEngine
{
	class CameraService;
	class ComponentRenderObject;
	class TransformComponent;

	class ServiceRender final : public Service
	{
	public:
		SET_TYPE_ID(ServiceID::Render);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void Register(const ComponentRenderObject* componentRenderObject);
		void Unregister(const ComponentRenderObject* componentRenderObject);

	private:
		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		struct Entry
		{
			const ComponentRenderObject* renderComponent = nullptr;
			const TransformComponent
		};
	};

}