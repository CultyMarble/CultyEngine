#pragma once

namespace CultyEngine
{
    class GameObject;

    namespace GameObjectFactory
    {
        void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
    }
}