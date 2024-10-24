#pragma once

#include "Common.h"

#include "Application.h"
#include "ApplicationState.h"

#include "Event.h"
#include "EventManager.h"

#include "SaveUtil.h"
#include "GameObjectFactory.h"

// Game World
#include "GameWorld.h"

// Services
#include "Service.h"
#include "CameraService.h"
#include "ServiceRender.h"

// Game Object
#include "GameObject.h"

// Components
#include "TypeID.h"
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "TransformComponent.h"
#include "ComponentRenderObject.h"

namespace CultyEngine
{
    Application& MainApplication();
}