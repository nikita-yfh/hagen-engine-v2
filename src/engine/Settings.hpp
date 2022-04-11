#pragma once

#include "box2d.h"
#include "String.hpp"
#include "JSONUtils.hpp"

struct Settings{
public:
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

		void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
		bool FromJSON(const rapidjson::Value &value);
	} graphics;

	struct AudioSettings {
		float soundVolume;
		float musicVolume;

		bool SetDefault();

		void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
		bool FromJSON(const rapidjson::Value &value);
	} audio;

	String language;

	bool SetDefault();

	void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool FromJSON(const rapidjson::Value &value);
};
