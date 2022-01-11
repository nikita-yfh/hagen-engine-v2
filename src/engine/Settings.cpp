#include "Settings.hpp"

#include "SDL.h"
#include "Logger.hpp"
#include "Utility.hpp"

Settings::Settings()
	:language(nullptr) {}

Settings::~Settings(){
	if(language)
		free(language);
}

int Settings::SetDefault(){
	language=cstrdup("en");
	return graphics.SetDefault() ||
			audio.SetDefault();
}
int Settings::GraphicsSettings::SetDefault(){
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
		Log(LEVEL_ERROR,SDL_GetError());
		return 1;
	}
	windowSize.Set(dm.w,dm.h);
	doubleBuffer=true;
	verticalSync=false;
	maxFPS=0;
	MSAASamples=4;
	windowMode=FULLSCREEN;
	return 0;
}
int Settings::AudioSettings::SetDefault(){
	soundVolume=1.0f;
	musicVolume=1.0f;
	return 0;
}

int Settings::GraphicsSettings::GetWindowFlags() const {
	int windowFlags=SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	switch(windowMode){
	case FULLSCREEN:
		windowFlags |= SDL_WINDOW_FULLSCREEN;
		break;
	case FULLSCREEN_DESKTOP:
		windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		break;
	}
	return windowFlags;
}

int Settings::SaveJSON(Archive *archive) const{
	return
		archive->SaveObject("graphics", &graphics) ||
		archive->SaveObject("audio",	&audio) ||
		archive->SaveString("language",	language);
}
int Settings::LoadJSON(Archive *archive){
	return
		archive->LoadObject("graphics",	&graphics) ||
		archive->LoadObject("audio",	&audio) ||
		archive->LoadString("language",	language);
}
int Settings::GraphicsSettings::SaveJSON(Archive *archive) const{
	return
		archive->SaveValue("doubleBuffer",	doubleBuffer) ||
		archive->SaveValue("verticalSync",	verticalSync) ||
		archive->SaveValue("maxFPS",		maxFPS) ||
		archive->SaveValue("MSAASamples",	MSAASamples) ||
		archive->SaveValue("windowMode",	windowMode) ||
		archive->PushSaveObject() ||
			archive->SaveValue("x",			windowSize.x) ||
			archive->SaveValue("y",			windowSize.y) ||
		archive->PopSaveObject("windowSize");
}
int Settings::GraphicsSettings::LoadJSON(Archive *archive){
	return
		archive->LoadValue("doubleBuffer",	doubleBuffer) ||
		archive->LoadValue("verticalSync",	verticalSync) ||
		archive->LoadValue("maxFPS",		maxFPS) ||
		archive->LoadValue("MSAASamples",	MSAASamples) ||
		archive->LoadValue("windowMode",	windowMode) ||
		archive->PushLoadObject("windowSize") ||
			archive->LoadValue("x",			windowSize.x) ||
			archive->LoadValue("y",			windowSize.y) ||
		archive->PopLoadObject();
}
int Settings::AudioSettings::SaveJSON(Archive *archive) const {
	return
		archive->SaveValue("soundVolume",	soundVolume) ||
		archive->SaveValue("musicVolume",	musicVolume);
}
int Settings::AudioSettings::LoadJSON(Archive *archive){
	return
		archive->LoadValue("soundVolume",	soundVolume) ||
		archive->LoadValue("musicVolume",	musicVolume);
}
