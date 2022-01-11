#pragma once

#include "box2d.h"
#include "Serializer.hpp"

struct Settings{
public:
	Settings();
	~Settings();

	struct GraphicsSettings {
		b2Vec2 windowSize;

		bool doubleBuffer;
		bool verticalSync;
		int maxFPS; //0 to disable limit

		int MSAASamples;

		enum WindowMode {
			FULLSCREEN,
			FULLSCREEN_DESKTOP,
			WINDOWED
		};
		int windowMode;

		int GetWindowFlags() const;

		int SetDefault();
		int SaveJSON(Archive *archive) const;
		int LoadJSON(Archive *archive);
	} graphics;

	struct AudioSettings {
		float soundVolume;
		float musicVolume;

		int SetDefault();

		int SaveJSON(Archive *archive) const;
		int LoadJSON(Archive *archive);
	} audio;

	struct InputSettings {
	};

	char *language;

	int SetDefault();

	int SaveJSON(Archive *archive) const;
	int LoadJSON(Archive *archive);
};
