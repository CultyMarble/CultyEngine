#include "Precompiled.h"
#include "AudioSystem.h"

using namespace DirectX;
using namespace CultyEngine;
using namespace CultyEngine::Audio;

namespace
{
    std::unique_ptr<AudioSystem> sAudioSystem;
}

void AudioSystem::StaticInitialize()
{
    ASSERT(sAudioSystem == nullptr, "AudioSystem: is already initialized!");
    sAudioSystem = std::make_unique<AudioSystem>();
    sAudioSystem->Initialize();
}

void AudioSystem::StaticTerminate()
{
    if (sAudioSystem == nullptr)
        return;

    sAudioSystem->Terminate();
    sAudioSystem.reset();
}

AudioSystem* AudioSystem::Get()
{
    ASSERT(sAudioSystem != nullptr, "AudioSystem: is not initialized!");
    return sAudioSystem.get();
}

AudioSystem::AudioSystem()
{
}

AudioSystem::~AudioSystem()
{
    ASSERT(sAudioSystem == nullptr, "AudioSystem: terminate must be called!");
}

void AudioSystem::Initialize()
{
    AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#if defined(_DEBUG)
    flags |= AudioEngine_Debug;
#endif

    mAudioEngine = new AudioEngine(flags);
}

void AudioSystem::Terminate()
{
    if (mAudioEngine != nullptr)
    {
        delete mAudioEngine;
        mAudioEngine = nullptr;
    }
}

void AudioSystem::Update()
{
    if (mAudioEngine != nullptr && mAudioEngine->Update())
    {
        // if no audio device active
        if (mAudioEngine->IsCriticalError())
        {
            LOG("AudioSystem: Critial Error, Shutting Down!");
            Terminate();
        }
    }
}

void AudioSystem::Suspend()
{
    ASSERT(sAudioSystem != nullptr, "AudioSystem: Audio Engine is not available!");
    mAudioEngine->Suspend();
}