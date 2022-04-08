#include "GameConfig.hpp"
#include <stdio.h>

Version::Version(const char *str){
	FromString(str);
}
bool Version::FromString(const char *str){
	if(sscanf(str, "%hu.%hu.%hu", &major, &minor, &patch) != 3){
		Log(LEVEL_ERROR, "Invalid version \"%s\"", str);
		return false;
	}
	return true;
}
String Version::ToString() const{
	return String::Format("%hu.%hu.%hu", major, minor, patch);
}

bool GameConfig::FromJSON(const rapidjson::Value &value){
	const char *versionStr;
	return
		jsonutils::CheckObject(value) &&
		jsonutils::GetMember(value, "name", name) &&
		jsonutils::GetMember(value, "icon", icon) &&
		jsonutils::GetMember(value, "version", versionStr) &&
		jsonutils::GetMember(value, "versionCode", versionCode) &&
		version.FromString(versionStr);
}

String GameConfig::GetFullGameName() const{
	return String::Format("%s %s", name.c_str(), version.ToString().c_str());
}
