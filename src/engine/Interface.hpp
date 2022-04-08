#pragma once

#include "glad/glad.h"
#include "imgui.h"
#include "SDL.h"
#include "ResourceManager.hpp"

class Window{
public:
	virtual ~Window() {}
	virtual bool Render() = 0;
	Window *next;
};

class Interface{
public:
	Interface(ResourceManager *resManager, SDL_Window*, SDL_GLContext);
	~Interface();

	void Render();
	void ProcessEvent(SDL_Event *e);
	void AddWindow(Window *window);
private:
	Window *windows;
	SDL_Window *window;
};
