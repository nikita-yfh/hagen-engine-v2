#include "RenderManager.hpp"

RenderManager::RenderManager(ResourceManager &resManager,
						const GameConfig &gameConfig,
						const Settings::GraphicsSettings &settings,
						Interface *&_interface)
		:interface(_interface){

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,
			settings.doubleBuffer);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,
			settings.MSAASamples);
	SDL_GL_SetSwapInterval(settings.verticalSync);

	const int windowNameSize=256;
	char windowName[windowNameSize];
	snprintf(windowName,windowNameSize,"%s %d.%d.%d",gameConfig.name,
		gameConfig.version.major,
		gameConfig.version.minor,
		gameConfig.version.patch);

	int flags=settings.GetWindowFlags();
	window=SDL_CreateWindow(windowName,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
							settings.windowSize.x,settings.windowSize.y,flags);

	if(!window){
		Log(LEVEL_ERROR,SDL_GetError());
		return;
	}

	context=SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window,context);

	gladLoadGL();

	interface=new Interface(resManager,window,context);
}
RenderManager::~RenderManager(){
	delete interface;
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);
}
void RenderManager::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	interface->Render();

	SDL_GL_SwapWindow(window);
}

