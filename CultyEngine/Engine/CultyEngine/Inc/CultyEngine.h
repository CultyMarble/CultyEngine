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

// Game Object
#include "GameObject.h"

// Services
#include "Service.h"
#include "ServiceCamera.h"
#include "ServicePhysics.h"
#include "ServiceRender.h"

// Components
#include "Component.h"
#include "ComponentAnimator.h"
#include "ComponentCamera.h"
#include "ComponentCameraFPS.h"
#include "ComponentMesh.h"
#include "ComponentModel.h"
#include "ComponentRenderObject.h"
#include "ComponentRigidbody.h"
#include "ComponentSoundBank.h"
#include "ComponentSoundEffect.h"
#include "ComponentTransform.h"

// Misc
#include "TypeID.h"

namespace CultyEngine
{
    Application& MainApplication();
}