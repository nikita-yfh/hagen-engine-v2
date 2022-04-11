#pragma once

#include "ResourceManager.hpp"
#include "Interface.hpp"
#include "Locale.hpp"
#include "JSONUtils.hpp"
#include "Settings.hpp"

struct HistoryEntry;

class DisplayModes{
public:
	bool Set(int displayIndex);
	void Render(const Locale &locale, Settings::GraphicsSettings::Size &size) const;
	~DisplayModes();
private:
	int count;
	SDL_DisplayMode *modes;
	String *strings;
};

class GUISettings : public Window{
public:
	GUISettings(Settings &settings);
	~GUISettings();

	void Render(const Locale &locale);
private:
	input::keycode prevKey;
	int selectedKey;
	Settings &settings;
	DisplayModes displayModes;
};
