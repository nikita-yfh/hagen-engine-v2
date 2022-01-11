#include "Engine.hpp"
#include "Logger.hpp"
#include "ResourceManager.hpp"
#include "SDL.h"
#include "GameConfig.hpp"

int main(int argc,char **argv){
	//Version hagenVersion("2.0.0");
	//Log(LEVEL_INFO,"Hagen Engine %s",hagenVersion.toString().c_str());

	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO))
		Log(LEVEL_FATAL,SDL_GetError());
	else
		Log(LEVEL_INFO,"SDL inited");

	argv[0]="game";
	const char*const*storages=argv;
	size_t storagesNum=argc;

	while(true){
		Engine engine(storages,storagesNum);
		/*if(engine.Init(storages,storagesNum)){
			Log(LEVEL_FATAL,"Failed to initilize engine");
			break;
		}*/
		engine.Run();
		if(engine.state==Engine::State::Quit)
			break;
		Log(LEVEL_INFO,"Restarting engine...");
	}

	SDL_Quit();
	return 0;
}
