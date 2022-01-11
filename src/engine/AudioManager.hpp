#pragma once

#include "SDL_mixer.h"

#include "Settings.hpp"
#include "Music.hpp"
#include "Sound.hpp"

class AudioManager{
public:
	AudioManager(const Settings::AudioSettings &settings);
	~AudioManager();

private:
	const Settings::AudioSettings &settings;
};
