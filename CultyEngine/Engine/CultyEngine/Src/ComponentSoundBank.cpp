#include "Precompiled.h"
#include "ComponentSoundBank.h"
#include "SaveUtil.h"

using namespace CultyEngine;
using namespace CultyEngine::Audio;

void ComponentSoundBank::Initialize()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    for (auto& data : mSoundEffects)
        data.second.soundId = sm->Load(data.second.fileName);
}

void ComponentSoundBank::Terminate()
{
    StopAll();
}

void ComponentSoundBank::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    if (!mSoundEffects.empty())
    {
        rapidjson::Value soundEffectsValue(rapidjson::kObjectType);
        for (auto& effect : mSoundEffects)
        {
            rapidjson::Value effectValue(rapidjson::kObjectType);
            SaveUtil::SaveString("FileName", effect.second.fileName.c_str(), doc, effectValue);
            SaveUtil::SaveBool("Looping", effect.second.isLooping, doc, effectValue);

            rapidjson::GenericStringRef<char> str(effect.first.c_str());
            soundEffectsValue.AddMember(str, effectValue, doc.GetAllocator());
        }
        soundEffectsValue.AddMember("SoundEffects", soundEffectsValue, doc.GetAllocator());
    }
    value.AddMember("ComponentSoundBank", componentValue, doc.GetAllocator());
}

void ComponentSoundBank::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("SoundEffects"))
    {
        auto soundEffects = value["SoundEffects"].GetObj();
        for (auto& effect : soundEffects)
        {
            SoundEffectData& data = mSoundEffects[effect.name.GetString()];

            if (effect.value.HasMember("FileName"))
                data.fileName = effect.value["FileName"].GetString();

            if (effect.value.HasMember("Looping"))
                data.isLooping = effect.value["Looping"].GetBool();
        }
    }
}

void ComponentSoundBank::DebugUI()
{
    if (ImGui::CollapsingHeader("SoundEffect"))
    {
        ImGui::Indent();
        for (auto& effect : mSoundEffects)
        {
            ImGui::Text(effect.first.c_str());

            std::string buttonLabel = "Play##" + effect.first;
            if (ImGui::Button(buttonLabel.c_str()))
                Play(effect.first);

            ImGui::SameLine();

            buttonLabel = "Stop##" + effect.first;
            if (ImGui::Button(buttonLabel.c_str()))
                Stop(effect.first);
        }
        ImGui::Unindent();
    }
}

void ComponentSoundBank::Play(const std::string& key)
{
    auto iter = mSoundEffects.find(key);
    if (iter != mSoundEffects.end())
        SoundEffectManager::Get()->Play(iter->second.soundId);
}

void ComponentSoundBank::Stop(const std::string& key)
{
    auto iter = mSoundEffects.find(key);
    if (iter != mSoundEffects.end())
        SoundEffectManager::Get()->Stop(iter->second.soundId);
}

void CultyEngine::ComponentSoundBank::StopAll()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    for (auto& effect : mSoundEffects)
        sm->Stop(effect.second.soundId);
}