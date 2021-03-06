#include "Engine.hpp"

#include "Logger.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Surface.hpp"
#include "GUIConsole.hpp"
#include "GUISettings.hpp"
#include "Level.hpp"
#include <chrono>

Engine::Engine(const char*const*storages, size_t num)
		: settings(inputConfig) {
	state = State::Error;

	if(!InitLua())
		return;

	for(int i = 0; i < num; i++)
		resManager.AddStorage(storages[i]);
	if(!resManager.HasStorages()){
		Log(LEVEL_FATAL, "No resources to load");
		return;
	}

	if(!resManager.LoadJSON("game.json", gameConfig))
		return;
	savesManager.Set(gameConfig.name);

	if(!resManager.LoadJSON("input.json", inputConfig))
		return;

	if(!LoadSettings())
		return;
	if(!CreateWindow(gameConfig))
		return;

	gladLoadGL();

	interface.AddWindow(new GUIConsole(resManager, L));
	interface.AddWindow(new GUISettings(this, settings));

	level = new Level();
	if(!resManager.LoadJSONExt("levels/1.json", *level))
		return;

	state = State::Run;
}

bool Engine::ApplySettingsNow(){
	state = State::Run;
	DestroyWindow();
	if(!CreateWindow(gameConfig))
		return LoadSettings();
	else
		return savesManager.SaveJSON("settings.json", settings);
}

bool Engine::LoadSettings() {
	if(!savesManager.LoadJSON("settings.json", settings))
		if(!settings.SetDefault() || !savesManager.SaveJSON("settings.json", settings))
			return false;
	return true;
}


bool Engine::CreateWindow(const GameConfig &config){
	const GraphicsSettings &graphics = settings.graphics;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,
			graphics.doubleBuffer);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,
			graphics.MSAASamples);
	SDL_GL_SetSwapInterval(graphics.verticalSync);

	int flags = graphics.GetWindowFlags();
	window = SDL_CreateWindow(config.GetFullGameName(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		graphics.windowSize.width, graphics.windowSize.height, graphics.GetWindowFlags());

	if(!window){
		Log(LEVEL_ERROR, SDL_GetError());
		return false;
	}

	Surface icon;
	if(resManager.LoadResource("ui/icon.png", &icon))
		icon.SetWindowIcon(window);
	else
		Log(LEVEL_WARNING, "Failed to set window icon");

	context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	interface.Create(resManager, window, context);

	return true;
}

Version Engine::GetVersion(){
	return Version("2.0.0");
}
bool Engine::BindLua(){
	return true;
}
bool Engine::InitLua(){
	L = luaL_newstate();
	if(!L){
		Log(LEVEL_FATAL, "Failed to create lua state");
		return false;
	}
	Log(LEVEL_INFO, "Created lua state");
	luaL_openlibs(L);
	BindLua();
	return true;
}
void Engine::CloseLua(){
	if(L){
		lua_close(L);
		Log(LEVEL_INFO, "Closed lua state");
	}
}

Engine::~Engine() {
	CloseLua();
	DestroyWindow();
	if(level)
		delete level;
}

void Engine::DestroyWindow(){
	interface.Destroy();
	if(window)
		SDL_DestroyWindow(window);
	if(context)
		SDL_GL_DeleteContext(context);
}

void Engine::ProcessEvents(){
	while (SDL_PollEvent(&event)) {
		interface.ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			state = State::Quit;
	}
}
void Engine::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(level)
		level->Render();
	interface.Render();

	SDL_GL_SwapWindow(window);
}

void Engine::Run() {
	do{
		if(state == State::Restart)
			ApplySettingsNow();

		auto prevTime = std::chrono::high_resolution_clock::now();
		while (state == State::Run) {
			ProcessEvents();
			Render();

			auto time = std::chrono::high_resolution_clock::now() - prevTime;
			prevTime = std::chrono::high_resolution_clock::now();
			float seconds = std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0f;

			if(level)
				level->Update(seconds);
		}
	} while (state == State::Restart);
}

