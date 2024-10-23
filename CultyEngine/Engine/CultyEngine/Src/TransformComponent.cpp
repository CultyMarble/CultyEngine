#include "Precompiled.h"
#include "TransformComponent.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void TransformComponent::DebugUI()
{
    ImGui::DragFloat3("Position", &position.x, 0.01f, -100.0f, 100.0f);
    ImGui::DragFloat4("Rotation", &rotation.x, 0.001f);
    ImGui::DragFloat3("Scale", &scale.x, 0.01f);

    SimpleDraw::AddTransform(GetMatrix4());
}