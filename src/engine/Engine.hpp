#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "SDL_mixer.h"
#include "SDL.h"
#include "glad/glad.h"
#include "GameConfig.hpp"
#include "Settings.hpp"
#include "ResourceManager.hpp"
#include "SavesManager.hpp"
#include "AudioManager.hpp"
#include "RenderManager.hpp"
#include "Interface.hpp"
#include "Controls.hpp"
#include "Locale.hpp"

class Engine{
public:
	Engine(const char*const*storages,size_t num);
	~Engine();

	void Run();

	enum class State{
		Run,
		Quit,
		Restart
	};

	State state;

	lua_State *L;
	GameConfig gameConfig;
	Settings settings;
	//Locale *loc;

private:
	int BindLua();
	int BindLuaAll();

	AudioManager *audioManager;
	ResourceManager *resManager;
	SavesManager *savesManager;
	RenderManager *renderManager;
	Interface *interface;
	EventManager *eventManager;
};
