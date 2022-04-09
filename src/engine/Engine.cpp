#include "Engine.hpp"

#include "Logger.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Surface.hpp"
#include "GUIConsole.hpp"
#include "GUISettings.hpp"

Engine::Engine(const char*const*storages,size_t num) {
	state = State::Error;

	if(!InitLua())
		return;

	for(int i = 0; i < num; i++)
		resManager.AddStorage(storages[i]);
	if(!resManager.HasStorages()){
		Log(LEVEL_FATAL, "No resources to load");
		return;
	}

	GameConfig gameConfig;
	if(!resManager.LoadJSON("game.json", gameConfig))
		return;
	savesManager.Set(gameConfig.name);

	if(!savesManager.LoadJSON("settings.json", settings))
		if(!settings.SetDefault() || !savesManager.SaveJSON("settings.json", settings))
			return;

	if(!CreateWindow(gameConfig))
		return;

	state = State::Run;
}
bool Engine::CreateWindow(const GameConfig &config){
	const Settings::GraphicsSettings &graphics = settings.graphics;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,
			graphics.doubleBuffer);
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
	SDL_GL_MakeCurrent(window,context);

	gladLoadGL();

	interface = new Interface(resManager, window, context, settings.language);
	interface->AddWindow(new GUIConsole(resManager, L));

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
	if(L)
		lua_close(L);
}

Engine::~Engine() {
	CloseLua();
	DestroyWindow();
}

void Engine::DestroyWindow(){
	if(window)
		SDL_DestroyWindow(window);
	if(context)
		SDL_GL_DeleteContext(context);
}

void Engine::ProcessEvents(){
	while (SDL_PollEvent(&event)) {
		interface->ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			state = State::Quit;
	}
}
void Engine::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	interface->Render();

	SDL_GL_SwapWindow(window);
}

void Engine::Run() {
	while (state == State::Run) {
		int t = SDL_GetTicks();
		ProcessEvents();
		Render();
	}
}

