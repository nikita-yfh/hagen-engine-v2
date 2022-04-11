#pragma once

#include "box2d.h"
#include "String.hpp"
#include "Input.hpp"
#include "JSONUtils.hpp"
#include "ResourceManager.hpp"

struct GraphicsSettings {
	struct Size{
		int width;
		int height;

		void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
		bool FromJSON(const rapidjson::Value &value);
	} windowSize;

	bool doubleBuffer;
	bool verticalSync;
	int maxFPS; // 0 to disable limit

	int MSAASamples;

	enum WindowMode {
		FULLSCREEN,
		FULLSCREEN_DESKTOP,
		WINDOWED
	};
	int windowMode;

	int GetWindowFlags() const;

	bool SetDefault();

	void SaveSettings(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool LoadSettings(const rapidjson::Value &value);
};

struct AudioSettings {
	float soundVolume;
	float musicVolume;

	bool SetDefault();

	void SaveSettings(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool LoadSettings(const rapidjson::Value &value);
};

struct Settings{
	Settings(input::InputConfig &config)
		: input(config) {}

	GraphicsSettings graphics;
	AudioSettings audio;
	input::InputConfig &input;

	String language;

	bool SetDefault();

	void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool FromJSON(const rapidjson::Value &value);
};
