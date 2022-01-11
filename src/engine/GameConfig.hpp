#pragma once

#include "Serializer.hpp"

struct Version {
	int major;
	int minor;
	int patch;

	int LoadJSON(Archive *archive);
};
struct GameConfig {
	GameConfig();
	~GameConfig();
	char name[256];
	char icon[256];
	Version version;
	int versionCode;

	char **languages;

	int LoadJSON(Archive *archive);
};
