#pragma once

#include "glad/glad.h"
#include "imgui.h"
#include "SDL.h"
#include "ResourceManager.hpp"
#include "Locale.hpp"

class Window{
public:
	virtual ~Window() {}
	virtual bool Render(const Locale &locale) = 0;
	Window *next;
};

class Interface{
public:
	Interface(ResourceManager *resManager, SDL_Window*, SDL_GLContext, const char *language);
	~Interface();

	void Render();
	void ProcessEvent(SDL_Event *e);
	void AddWindow(Window *window);
private:
	Locale locale;
	Window *windows;
	SDL_Window *window;
};
