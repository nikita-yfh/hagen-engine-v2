#include "GUISettings.hpp"

bool DisplayModes::Set(int displayIndex){
	count = SDL_GetNumDisplayModes(displayIndex);
	if(count <= 0){
		Log(LEVEL_ERROR, SDL_GetError());
		count = 0;
		return false;
	}
	modes = new SDL_DisplayMode[count];
	strings = new String[count];
	SDL_DisplayMode *prev = nullptr;
	for(int i = 0; i < count; i++){
		if(SDL_GetDisplayMode(displayIndex, i, &modes[i]) != 0) {
			Log(LEVEL_ERROR, SDL_GetError());
			this->~DisplayModes();
			return false;
		}
	}
	return true;
}
void DisplayModes::Render(const Locale &locale, Settings::GraphicsSettings::Size &size) const {
	char currentSizeStr[32];
	sprintf(currentSizeStr, "%d x %d", size.width, size.height);
	if(ImGui::BeginCombo(locale["settings.windowSize"], currentSizeStr)){
		for(int i = 0; i < count; i++){
			SDL_DisplayMode &mode = modes[i];
			char sizeStr[32];
			sprintf(sizeStr, "%d x %d, %dhz", mode.w, mode.h, mode.refresh_rate);
			if(ImGui::Selectable(sizeStr, strcmp(currentSizeStr, sizeStr) == 0)){
				size.width = mode.w;
				size.height = mode.h;
			}
		}
		ImGui::EndCombo();
	}
}

DisplayModes::~DisplayModes(){
	if(modes && count)
		delete [] modes;
	count = 0;
}

GUISettings::GUISettings(Settings &_settings)
			: settings(_settings) {
	displayModes.Set(0);
}
GUISettings::~GUISettings(){
}

void GUISettings::Render(const Locale &locale) {
	ImGui::Begin(locale["settings.title"], &shown);
	ImGui::BeginTabBar("tabs");
	ImGui::PushItemWidth(ImGui::GetFontSize()*15.0f);
	if(ImGui::BeginTabItem(locale["settings.graphics"])){
		Settings::GraphicsSettings &graphics = settings.graphics;
		const char *windowModes[] = {
			locale["settings.windowModes.fullscreen"],
			locale["settings.windowModes.fullscreenDesktop"],
			locale["settings.windowModes.windowed"]
		};
		displayModes.Render(locale, graphics.windowSize);
		if(ImGui::BeginCombo(locale["settings.windowMode"], windowModes[graphics.windowMode])){
			for(int i = 0; i < 3; i++)
				if(ImGui::Selectable(windowModes[i], graphics.windowMode == i))
					graphics.windowMode = i;
			ImGui::EndCombo();
		}
		bool limitFPS = (graphics.maxFPS != 0);
		ImGui::Checkbox(locale["settings.doubleBuffer"], &graphics.doubleBuffer);
		ImGui::Checkbox(locale["settings.verticalSync"], &graphics.verticalSync);
		ImGui::Checkbox(locale["settings.limitFPS"], &limitFPS);
		if(limitFPS != (graphics.maxFPS != 0))
			graphics.maxFPS = limitFPS ? 60.0f : 0.0f;
		if(limitFPS){
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetFontSize()*8.0f);
			ImGui::InputInt(locale["settings.maxFPS"], &graphics.maxFPS);
		}
		ImGui::EndTabItem();
	}
	if(ImGui::BeginTabItem(locale["settings.audio"])){
		Settings::AudioSettings &audio = settings.audio;
		ImGui::SliderFloat(locale["settings.sound"], &audio.soundVolume, 0.0f, 1.0f, "");
		ImGui::SliderFloat(locale["settings.music"], &audio.musicVolume, 0.0f, 1.0f, "");
		ImGui::EndTabItem();
	}
	if(ImGui::BeginTabItem(locale["settings.input"])){
		ImGui::EndTabItem();
	}
	if(ImGui::BeginTabItem(locale["settings.game"])){
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();	
	ImGui::End();
}
