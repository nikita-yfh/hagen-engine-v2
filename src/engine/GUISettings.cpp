#include "GUISettings.hpp"
#include "Engine.hpp"

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
	
	int c = count;
	for(int i = 0, j = 0; i < c; i++, j++){
		if(SDL_GetDisplayMode(displayIndex, i, &modes[j]) != 0) {
			Log(LEVEL_ERROR, SDL_GetError());
			this->~DisplayModes();
			return false;
		}
		if(j > 0 && modes[j-1].w == modes[j].w && modes[j-1].h == modes[j].h){
			j--;
			count--;
		}
	}
	return true;
}
void DisplayModes::Render(const Locale &locale, GraphicsSettings::Size &size) const {
	char currentSizeStr[32];
	sprintf(currentSizeStr, "%d x %d", size.width, size.height);
	if(ImGui::BeginCombo(locale["settings.windowSize"], currentSizeStr)){
		for(int i = 0; i < count; i++){
			SDL_DisplayMode &mode = modes[i];
			char sizeStr[32];
			sprintf(sizeStr, "%d x %d", mode.w, mode.h);
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

GUISettings::GUISettings(Engine *_engine, Settings &_settings)
			: settings(_settings), engine(_engine) {
	displayModes.Set(0);
	selectedKey = 0;
}
GUISettings::~GUISettings(){
}

void GUISettings::Render(const Locale &locale) {
	ImVec2 size(ImGui::GetFontSize()*30.0f, ImGui::GetFontSize()*20.0f);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin(locale["settings.title"], &shown);
	ImVec2 reserve(0.0f, -ImGui::GetStyle().ItemSpacing.y -
		ImGui::GetFrameHeightWithSpacing());
	ImGui::BeginChild("settings", reserve, false);
	ImGui::BeginTabBar("tabs");
	ImGui::PushItemWidth(ImGui::GetFontSize()*15.0f);
	if(ImGui::BeginTabItem(locale["settings.graphics"])){
		GraphicsSettings &graphics = settings.graphics;
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
		AudioSettings &audio = settings.audio;
		ImGui::SliderFloat(locale["settings.sound"], &audio.soundVolume, 0.0f, 1.0f, "");
		ImGui::SliderFloat(locale["settings.music"], &audio.musicVolume, 0.0f, 1.0f, "");
		ImGui::EndTabItem();
	}
	if(ImGui::BeginTabItem(locale["settings.input"])){
		input::InputConfig &input = settings.input;
		ImGui::BeginChild("keys", ImVec2(0.0f, 0.0f), true);
		for(int i = 0; i < input.keyCount; i++){
			char str[256];
			snprintf(str, 256, "settings.keyBindings.%s", input.keys[i].name.c_str());
			if(ImGui::Selectable(locale[str], selectedKey == i,
						ImGuiSelectableFlags_AllowDoubleClick)){
				selectedKey = i;
				if(ImGui::IsMouseDoubleClicked(0)){
					prevKey = input::GetPressedCode();
					ImGui::OpenPopup("bind");
				}
			}
			ImGui::SameLine(ImGui::GetFontSize()*12.0f);
			ImGui::Text(input::KeyToStr(input.keys[i].key));
		}
		if(ImGui::BeginPopupModal("bind", nullptr,
					ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)){
			ImGui::Text(locale["settings.bind"]);
			input::keycode key = input::GetPressedCode();
			if(key != prevKey && key != input::NOT_PRESSED){
				input.keys[selectedKey].key = key;
				for(int i = 0; i < input.keyCount; i++)
					if(selectedKey != i && input.keys[selectedKey].key == input.keys[i].key)
						input.keys[i].key = input::NOT_PRESSED;
				ImGui::CloseCurrentPopup();
			}
			prevKey = key;
			ImGui::EndPopup();
		}
		ImGui::EndChild();
		ImGui::EndTabItem();

	}
	if(ImGui::BeginTabItem(locale["settings.game"])){
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();	
	ImGui::EndChild();
	ImGui::Separator();
	if(ImGui::Button(locale["ok"])){
		engine->ApplySettings();
		Hide();
	}
	ImGui::SameLine();
	if(ImGui::Button(locale["apply"]))
		engine->ApplySettings();
	ImGui::SameLine();
	if(ImGui::Button(locale["cancel"])){
		engine->LoadSettings();
		Hide();
	}
	ImGui::SameLine();
	if(ImGui::Button(locale["settings.default"]))
		settings.SetDefault();
	ImGui::SameLine();
	ImGui::End();
}
