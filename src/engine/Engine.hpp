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
#include "Input.hpp"

class Engine{
public:
	Engine(const char*const*storages, size_t num);
	~Engine();

	static Version GetVersion();

	inline bool HasError() const{
		return state == State::Error;
	}
	inline void ApplySettings() {
		// You can't restart engine
		// in ImGui frame
		state = State::Restart;
	}

	void Run();

	enum class State{
		Run,
		Quit,
		Restart,
		Error
	};

	bool LoadSettings();

private:
	bool CreateWindow(const GameConfig &config);
	void DestroyWindow();
	bool ApplySettingsNow();

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
	GameConfig gameConfig;
	input::InputConfig inputConfig;
	Interface interface;
};
