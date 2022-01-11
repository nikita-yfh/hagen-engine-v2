#include "Engine.hpp"

#include "SDL_image.h"
#include "Logger.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Locale.hpp"
#include "Texture.hpp"
#include "GUIConsole.hpp"
#include "GUISettings.hpp"
#include "Sound.hpp"
#include "Music.hpp"

Engine::Engine(const char*const*storages,size_t num) {
	state=State::Run;

	resManager=new ResourceManager(storages,num);

	if(resManager->LoadJSON("game.json",&gameConfig))
		return;
	savesManager=new SavesManager(gameConfig.name);

	if(savesManager->LoadJSON("settings.json",&settings)){
		if(settings.SetDefault() ||
				savesManager->SaveJSON("settings.json",&settings))
			return;
	}
	//loc=resManager.LoadResource<Locale>("locales/"+settings.language+"/locale.json");
	//if(loc==nullptr)
	//	return;

	renderManager=new RenderManager(*resManager,gameConfig,settings.graphics,interface);

	eventManager=new EventManager(*interface);
	audioManager=new AudioManager(settings.audio);

	L = luaL_newstate();
	Log(LEVEL_INFO,"Created lua state");

	luaL_openlibs(L);

	BindLuaAll();

//	interface.AddWindow(new GUIConsole (*loc,L));
	//interface.AddWindow(new GUISettings(*loc,settings,gameConfig));

	Log(LEVEL_INFO,"Engine initilized");
}
int Engine::BindLuaAll(){
	return 0;
}
int Engine::BindLua(){
	return 0;
}
Engine::~Engine() {

	lua_close(L);
}


void Engine::Run() {
	while (state==State::Run) {
		int t=SDL_GetTicks();
		if(eventManager->ProcessEvents())
			state=State::Quit;
		renderManager->Render();
	}
}

