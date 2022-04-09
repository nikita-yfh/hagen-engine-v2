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

class Engine{
public:
	Engine(const char*const*storages, size_t num);
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

private:
	bool CreateWindow(const GameConfig &config);
	void DestroyWindow();

	void ProcessEvents();
	void Render();

	bool InitLua();
	bool BindLua();
	void CloseLua();

	State state;

	SDL_Event event;
	SDL_Window *window;
	SDL_GLContext context;
	lua_State *L;
	Settings settings;

	ResourceManager resManager;
	SavesManager savesManager;
	Interface *interface;
};
