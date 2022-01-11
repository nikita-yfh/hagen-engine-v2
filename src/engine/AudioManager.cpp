#include "AudioManager.hpp"

#include "Logger.hpp"

AudioManager::AudioManager(const Settings::AudioSettings &_settings)
		:settings(_settings){
	int mix_flags=MIX_INIT_MP3|MIX_INIT_MOD;
	if((Mix_Init(mix_flags)&mix_flags)!=mix_flags ||
			Mix_OpenAudio(22050,AUDIO_S16SYS,2,640)){
		Log(LEVEL_ERROR,SDL_GetError());
	}else
		Log(LEVEL_INFO,"Music inited");
}
AudioManager::~AudioManager() {
	Mix_CloseAudio();
	Mix_Quit();
}
