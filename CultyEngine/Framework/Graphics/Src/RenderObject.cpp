#include "Precompiled.h"
#include "RenderObject.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}