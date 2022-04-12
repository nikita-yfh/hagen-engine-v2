#pragma once

#include "glad/glad.h"
#include "imgui.h"
#include "SDL.h"
#include "ResourceManager.hpp"
#include "Locale.hpp"

class Window{
public:
	Window() : shown(true) {}
	virtual ~Window() {}
	virtual void Render(const Locale &locale) = 0;

	inline void Show(){
		shown = true;
	}
	inline void Hide(){
		shown = false;
	}
	bool shown;

	Window *next;
};

class Interface{
public:
	Interface();
	~Interface();

	void Render();
	void ProcessEvent(SDL_Event *e);
	void AddWindow(Window *window);

	bool Create(ResourceManager &resManager, SDL_Window*, SDL_GLContext);
	void Destroy();
private:
	Locale locale;
	Window *windows;
	SDL_Window *window;
};
