#pragma once

#include "String.hpp"
#include "JSONUtils.hpp"

struct Version {
	Version() {}
	Version(const char *str);

	unsigned short major;
	unsigned short minor;
	unsigned short patch;

	bool FromString(const char *str);
	String ToString() const;
};

struct GameConfig {
	String name;
	String icon;

	Version version;
	int versionCode;

	String GetFullGameName() const;

	bool FromJSON(const rapidjson::Value &value);
};
