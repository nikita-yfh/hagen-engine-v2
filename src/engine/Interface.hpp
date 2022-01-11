#pragma once

#include "glad/glad.h"
#include "imgui.h"
#include "SDL.h"
#include "Serializer.hpp"
#include "ResourceManager.hpp"

class Window{
public:
	Window() : shown(true), next(nullptr) {}
	virtual ~Window() {}

	void Show();
	void Hide();

	virtual void Render() = 0;

	bool shown;

	Window *next;
};
class Interface{
public:
	Interface(ResourceManager &resManager,SDL_Window*,SDL_GLContext);
	~Interface();

	void Render();
	void ProcessEvent(SDL_Event *e);
	void AddWindow(Window *window);
private:
	Window *windows;
	SDL_Window *window;
};
