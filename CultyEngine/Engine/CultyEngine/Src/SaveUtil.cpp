#include "Precompiled.h"
#include "SaveUtil.h"

using namespace CultyEngine;

void SaveUtil::SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	member.AddMember(str, value, doc.GetAllocator());
}

void SaveUtil::SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& member)
{
}

void SaveUtil::SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	member.AddMember(str, value, doc.GetAllocator());
}

void SaveUtil::SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	rapidjson::GenericStringRef<char> strValue(key);
	member.AddMember(str, strValue, doc.GetAllocator());
}

void SaveUtil::SaveStringArray(const char* key, const std::vector<std::string>& value, rapidjson::Document& doc, rapidjson::Value& member)
{
	rapidjson::GenericStringRef<char> str(key);
	rapidjson::Value strArray(rapidjson::kArrayType);
	for (const std::string& v : value)
	{
		rapidjson::GenericStringRef<char> strValue(v.c_str());
		strArray.PushBack(strValue, doc.GetAllocator());
	}
	member.AddMember(str, strArray, doc.GetAllocator());
}

void SaveUtil::SaveVector3(const char* key, const Vector3& value, rapidjson::Document& doc, rapidjson::Value& member)
{

}

void SaveUtil::SaveQuaternion(const char* key, const Quaternion& value, rapidjson::Document& doc, rapidjson::Value& member)
{
}

void SaveUtil::SaveColor(const char* key, const Color& value, rapidjson::Document& doc, rapidjson::Value& member)
{

}