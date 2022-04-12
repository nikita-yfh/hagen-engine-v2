#pragma once

#include "Interface.hpp"
#include "Locale.hpp"
#include "JSONUtils.hpp"
#include "Settings.hpp"

struct HistoryEntry;
class Engine;

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
	GUISettings(Engine *engine, Settings &settings);
	~GUISettings();

	void Render(const Locale &locale);
private:
	input::keycode prevKey;
	int selectedKey;
	Settings &settings;
	Engine *engine;
	DisplayModes displayModes;
};
