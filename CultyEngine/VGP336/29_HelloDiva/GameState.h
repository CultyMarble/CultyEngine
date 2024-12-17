#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    CultyEngine::GameWorld mGameWorld;

private:
    using NoteList = std::vector<std::tuple<float, std::string, std::string>>; // {time, templatePath, componentsJson}

    NoteList mNotes;
    float mElapsedTime = 0.0f;
};