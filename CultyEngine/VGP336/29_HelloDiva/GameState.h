#pragma once
#include <CultyEngine/Inc/CultyEngine.h>
#include <queue>

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
    void SpawnNote();
    void HandleTrackedNote();
    void HandlePlayerInput();

private:
    // Define valid buttons
    std::vector<int> mValidButtons =
    {
        CultyEngine::Input::PS4_BUTTON_SQUARE,
        CultyEngine::Input::PS4_BUTTON_X,
        CultyEngine::Input::PS4_BUTTON_CIRCLE,
        CultyEngine::Input::PS4_BUTTON_TRIANGLE
    };

    using NoteList = std::vector<std::tuple<float, std::string, std::string>>;
    using NoteQueue = std::queue<CultyEngine::GameObjectHandle>;

    NoteList mNotes;
    NoteQueue mNoteQueue;

    float mElapsedTime = 0.0f;
    CultyEngine::GameObjectHandle mTrackedNote;
};