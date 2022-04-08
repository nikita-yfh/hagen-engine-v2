#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "SDL.h"
#include "glad/glad.h"
#include "GameConfig.hpp"
#include "Settings.hpp"
#include "ResourceManager.hpp"
#include "SavesManager.hpp"
#include "Interface.hpp"
#include "Locale.hpp"

class Engine{
public:
	Engine(const char*const*storages,size_t num);
	~Engine();

	static Version GetVersion();

	inline bool HasError() const{
		return state == State::Error;
	}
	inline bool NeedQuit() const{
		return state == State::Quit;
	}

	void Run();

	enum class State{
		Run,
		Quit,
		Restart,
		Error
	};

	lua_State *L;
	Settings settings;
	Locale *loc;
private:
	bool CreateWindow(const GameConfig &config);
	void DestroyWindow();

	void ProcessEvents();
	void Render();

	bool BindLua();
	bool BindLuaAll();

	State state;

	SDL_Event event;
	SDL_Window *window;
	SDL_GLContext context;

	ResourceManager *resManager;
	SavesManager *savesManager;
	Interface *interface;
};
