#pragma once
#include "SDL.h"
#include "JSONUtils.hpp"

namespace input{

typedef uint16_t keycode;

enum{
	KEYBOARD_KEYS = 0,
	MOUSE_KEYS = 512,
	KEY_COUNT = 517,
	NOT_PRESSED = UINT16_MAX
};

keycode GetPressedCode();

// key names for GUI
const char *KeyToStr(keycode key);
keycode StrToKey(const char *name);

bool IsPressed(keycode key);

struct Key {
	String name;
	keycode defaultKey;
	keycode key;
};

struct InputConfig {
	InputConfig();
	~InputConfig();

	keycode GetKey(const char *str) const;

	bool SetDefault();

	void SaveSettings(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool LoadSettings(const rapidjson::Value &value);

	bool FromJSON(const rapidjson::Value &value);

	Key *keys;
	size_t keyCount;
};

}
