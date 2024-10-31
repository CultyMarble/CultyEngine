#include "Precompiled.h"
#include "ComponentModel.h"
#include "SaveUtil.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentModel::Initialize()
{
	ModelManager* mm = ModelManager::Get();
	mModelID = mm->GetModelID(mFileName);
	if (mm->GetModel(mModelID) == nullptr)
	{
		mModelID = mm->LoadModelID(mFileName);
		for (const std::string& animation : mAnimations)
		{
			mm->AddAnimation(mModelID, animation);
		}
	}

	ASSERT(mm->GetModel(mModelID) != nullptr, );
}

void ComponentModel::Terminate()
{

}

void ComponentModel::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	ComponentRenderObject::Serialize(doc, value);

	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveString("FileName", mFileName.c_str(), doc, componentValue);
	SaveUtil::SaveStringArray("Animations", mAnimations, doc, componentValue);
	value.AddMember("ComponentModel", componentValue, doc.GetAllocator());
}

void ComponentModel::Deserialize(const rapidjson::Value& value)
{
	ComponentRenderObject::Deserialize(value);

	if (value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if (value.HasMember("Animations"))
	{
		mFileName = value["FileName"].GetString();
	}
}

ModelID ComponentModel::GetModelID() const
{
	return mModelID;
}

const Model& ComponentModel::GetModel() const
{
	return *ModelManager::Get()->GetModel(mModelID);
}
