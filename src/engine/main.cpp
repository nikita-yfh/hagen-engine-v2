#include "Engine.hpp"
#include "Logger.hpp"
#include "ResourceManager.hpp"
#include "SDL.h"
#include "GameConfig.hpp"

int main(int argc,char **argv){
	Log(LEVEL_INFO,"Hagen Engine %s",Engine::GetVersion().ToString().c_str());

	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO))
		Log(LEVEL_FATAL, SDL_GetError());
	else
		Log(LEVEL_INFO, "SDL inited");

	const char*const*storages = argv + 1;
	size_t storagesNum = argc - 1;

	while(true){
		Engine engine(storages, storagesNum);
		if(engine.HasError()){
			Log(LEVEL_FATAL,"Failed to initilize engine");
			break;
		}
		Log(LEVEL_INFO,"Engine initilized");
		engine.Run();
		if(engine.NeedQuit())
			break;
		Log(LEVEL_INFO,"Restarting engine...");
	}

	SDL_Quit();
	return 0;
}
