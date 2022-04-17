#include "Settings.hpp"

#include "SDL.h"
#include "Logger.hpp"
#include "Utility.hpp"

static const char *windowModes[] = {
	"fullscreen",
	"fullscreenDesktop",
	"windowed"
};

bool Settings::SetDefault(){
	language = "en";
	return graphics.SetDefault() &&
			audio.SetDefault() &&
			input.SetDefault();
}
bool GraphicsSettings::SetDefault(){
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
		Log(LEVEL_ERROR,SDL_GetError());
		return false;
	}
	windowSize.width = dm.w;
	windowSize.height = dm.h;
	doubleBuffer = true;
	verticalSync = false;
	maxFPS = 0;
	MSAASamples = 4;
	windowMode = FULLSCREEN;
	return true;
}
bool AudioSettings::SetDefault(){
	soundVolume = 1.0f;
	musicVolume = 1.0f;
	return true;
}

int GraphicsSettings::GetWindowFlags() const {
	int windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
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

void Settings::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.SetObject();
	graphics.SaveSettings(value, allocator);
	audio.SaveSettings(value, allocator);
	input.SaveSettings(value, allocator);
	value.AddMember("language",	jsonutils::ToJSON(language), allocator);
}
bool Settings::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::CheckObject(value) &&
		graphics.LoadSettings(value) &&
		audio.LoadSettings(value) &&
		input.LoadSettings(value) &&
		jsonutils::GetMember(value, "language",	language);
}
void GraphicsSettings::SaveSettings(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("windowSize", jsonutils::ToJSON(windowSize, allocator), allocator);
	value.AddMember("doubleBuffer",	doubleBuffer, allocator);
	value.AddMember("verticalSync", verticalSync, allocator);
	value.AddMember("maxFPS", maxFPS, allocator);
	value.AddMember("MSAASamples", MSAASamples, allocator);
	value.AddMember("windowMode", jsonutils::StringType(windowModes[windowMode]), allocator);
}
bool GraphicsSettings::LoadSettings(const rapidjson::Value &value){
	const char *windowModeStr;
	return
		jsonutils::GetMember(value, "windowSize", windowSize) &&
		jsonutils::GetMember(value, "doubleBuffer", doubleBuffer) &&
		jsonutils::GetMember(value, "verticalSync", verticalSync) &&
		jsonutils::GetMember(value, "maxFPS", maxFPS) &&
		jsonutils::GetMember(value, "MSAASamples", MSAASamples) &&
		jsonutils::GetMember(value, "windowMode", windowModeStr) &&
		(windowMode = jsonutils::GetEnum(windowModeStr, windowModes, 3)) != -1;
}
void GraphicsSettings::Size::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.SetArray();
	value.PushBack(width, allocator);
	value.PushBack(height, allocator);
}
bool GraphicsSettings::Size::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::CheckArray(value, 2) &&
		jsonutils::GetArrayMember(value, 0, width) &&
		jsonutils::GetArrayMember(value, 1, height);
}
void AudioSettings::SaveSettings(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("soundVolume", soundVolume, allocator);
	value.AddMember("musicVolume", musicVolume, allocator);
}
bool AudioSettings::LoadSettings(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "soundVolume", soundVolume) &&
		jsonutils::GetMember(value, "musicVolume", musicVolume);
}
