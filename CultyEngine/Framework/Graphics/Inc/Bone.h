#pragma once
#include "Common.h"

namespace CultyEngine::Graphics
{
	struct Bone
	{
		std::string name;
		int index = -1;

		Bone* parent = nullptr;
		int parentIndex = -1;

		std::vector<Bone*> children;
		std::vector<int> childrenIndices;

		MathC::Matrix4 toParentTransform;
		MathC::Matrix4 offsetTransform;
	};
}