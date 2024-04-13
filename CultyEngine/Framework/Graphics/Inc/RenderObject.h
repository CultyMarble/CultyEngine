#pragma once
#include "MeshBuffer.h"
#include "TextureManager.h"
#include "Transform.h"

namespace CultyEngine::Graphics
{
	struct RenderObject
	{
		void Terminate();

		Transform transform;
		MeshBuffer meshBuffer;

		TextureID diffuseTextureID;
	};
}