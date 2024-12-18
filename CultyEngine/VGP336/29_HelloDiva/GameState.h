#pragma once
#include <CultyEngine/Inc/CultyEngine.h>
#include <queue>
#include <vector>
#include <string>

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
    void LoadTimeline(const std::filesystem::path& timelineFile);
    void SpawnNote();
    void HandleTrackedNote();
    void HandlePlayerInput();

    void SpawnFeedbackPopup(const std::string& templatePath, const CultyEngine::MathC::Vector2& position, float lifetime);

    // Frame extraction and cleanup
    void ExtractFramesFromVideo(const std::string& videoPath, const std::string& outputFolder);
    void CleanFrameFolder(const std::string& outputFolder);
    void LoadPPMFramesAsTextures(const std::string& folderPath);

    // Texture handling
    std::vector<CultyEngine::Graphics::Texture*> mTextures;
    float mFrameTime = 1.0f / 30.0f; // For 60 FPS
    float mCurrentFrameTime = 0.0f;
    size_t mCurrentFrameIndex = 0;

    void CreateFullscreenUISpriteWithTexture();
    void UpdateFullscreenUISpriteTexture();

private:
    // Define valid input buttons
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
    CultyEngine::GameObjectHandle mFullscreenUISpriteHandle;

    // Audio System variables
    CultyEngine::Audio::SoundId mSoundIdHit_Cool;
    CultyEngine::Audio::SoundId mSoundIdHit_Empty;
    CultyEngine::Audio::SoundId mSoundIdHit_Regular;
    CultyEngine::Audio::SoundId mSoundIdMiss;

    CultyEngine::Audio::SoundId mBackgroundMusic;
};