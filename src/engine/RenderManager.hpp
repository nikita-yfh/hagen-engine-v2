#pragma once

#include "glad.h"
#include "SDL.h"

#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Settings.hpp"
#include "Interface.hpp"

class RenderManager{
public:
	RenderManager(ResourceManager &resManager,
				const GameConfig &gameConfig,
				const Settings::GraphicsSettings &settings,
				Interface *&interface);
	~RenderManager();

	void Render();
private:
	SDL_Window *window;
	SDL_GLContext context;

	Interface *interface;
};
