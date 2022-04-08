#include "Engine.hpp"

#include "SDL_image.h"
#include "Logger.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Locale.hpp"
#include "Texture.hpp"
#include "GUIConsole.hpp"
#include "GUISettings.hpp"

Engine::Engine(const char*const*storages,size_t num) {
	state = State::Error;
	L = nullptr;

	resManager = new ResourceManager();
	for(int i = 0; i < num; i++)
		resManager->AddStorage(storages[i]);
	if(!resManager->HasStorages()){
		Log(LEVEL_FATAL, "No resources to load");
		return;
	}

	if(!resManager->LoadJSON("game.json", gameConfig))
		return;
	savesManager = new SavesManager(gameConfig.name);

	if(!savesManager->LoadJSON("settings.json", settings))
		if(!settings.SetDefault() || !savesManager->SaveJSON("settings.json", settings))
			return;

	loc = resManager->LoadResource<Locale>(String::Format("locales/%s/locale.json",
				settings.language.c_str()));
	if(loc == nullptr)
		return;

	renderManager = new RenderManager(*resManager, gameConfig, settings.graphics, interface);

	eventManager = new EventManager(*interface);

	L = luaL_newstate();
	Log(LEVEL_INFO,"Created lua state");

	luaL_openlibs(L);

	BindLuaAll();

//	interface.AddWindow(new GUIConsole (*loc,L));
	//interface.AddWindow(new GUISettings(*loc,settings,gameConfig));

	state = State::Run;
}
Version Engine::GetVersion(){
	return Version("2.0.0");
}
bool Engine::BindLuaAll(){
	return true;
}
bool Engine::BindLua(){
	return true;
}
Engine::~Engine() {
	if(L)
		lua_close(L);
	if(savesManager)
		delete savesManager;
	if(resManager)
		delete resManager;
	if(renderManager)
		delete renderManager;
	if(eventManager)
		delete eventManager;
}


void Engine::Run() {
	while (state == State::Run) {
		int t = SDL_GetTicks();
		if(eventManager->ProcessEvents())
			state = State::Quit;
		renderManager->Render();
	}
}

