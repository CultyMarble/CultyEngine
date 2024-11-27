#include "Precompiled.h"
#include "ComponentSoundEffect.h"

#include "SaveUtil.h"

using namespace CultyEngine;
using namespace CultyEngine::Audio;

void ComponentSoundEffect::Initialize()
{
    ASSERT(!mFileName.empty(), "ComponentSoundEffect: no sound file loaded");
    mSoundId = SoundEffectManager::Get()->Load(mFileName);
}

void ComponentSoundEffect::Terminate()
{
    Stop();
}

void ComponentSoundEffect::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    SaveUtil::SaveString("FileName", mFileName.c_str(), doc, componentValue);
    SaveUtil::SaveBool("Looping", mLooping, doc, componentValue);
    value.AddMember("ComponentSoundEffect", componentValue, doc.GetAllocator());
}

void ComponentSoundEffect::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("FileName"))
    {
        mFileName = value["FileName"].GetString();
    }
    if (value.HasMember("Looping"))
    {
        mLooping = value["Looping"].GetBool();
    }
}

void ComponentSoundEffect::DebugUI()
{
    ImGui::Text(mFileName.c_str());

    if (ImGui::Button("Play"))
        Play();

    if (ImGui::Button("Stop"))
        Stop();
}

void ComponentSoundEffect::Play()
{
    SoundEffectManager::Get()->Play(mSoundId);
}

void ComponentSoundEffect::Stop()
{
    SoundEffectManager::Get()->Stop(mSoundId);
}