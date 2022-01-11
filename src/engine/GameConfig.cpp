#include "GameConfig.hpp"

GameConfig::GameConfig()
	: languages(nullptr) {}
GameConfig::~GameConfig(){
	if(languages){
		while(*languages) delete *(languages++);
		free(languages);
	}
}
int Version::LoadJSON(Archive *archive){
	return
		archive->LoadValue("major",			major) ||
		archive->LoadValue("minor",			minor) ||
		archive->LoadValue("patch",			patch);
}
int GameConfig::LoadJSON(Archive *archive){
	return
		archive->LoadString("name",			name) ||
		archive->LoadString("icon",			icon) ||
		archive->LoadObject("version",		&version) ||
		archive->LoadValue("versionCode",	versionCode) ||
		archive->LoadStringArray("languages", languages);
}
