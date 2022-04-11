#pragma once

#include "SavesManager.hpp"
#include "Interface.hpp"
#include "Locale.hpp"
#include "JSONUtils.hpp"
#include "Settings.hpp"

struct HistoryEntry;

class DisplayModes{
public:
	bool Set(int displayIndex);
	void Render(const Locale &locale, GraphicsSettings::Size &size) const;
	~DisplayModes();
private:
	int count;
	SDL_DisplayMode *modes;
	String *strings;
};

class GUISettings : public Window{
public:
	GUISettings(SavesManager &savesManager, Settings &settings);
	~GUISettings();

	void Render(const Locale &locale);
private:
	void DiscardSettings();
	void SaveSettings();

	input::keycode prevKey;
	int selectedKey;
	Settings &settings;
	SavesManager &savesManager;
	DisplayModes displayModes;
};
