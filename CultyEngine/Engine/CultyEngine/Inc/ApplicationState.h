#pragma once

namespace CultyEngine
{
    class ApplicationState
    {
    public:
        virtual ~ApplicationState() = default;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltaTime) {}
        virtual void Render() {}
        virtual void DebugUI() {}
    };
}