#pragma once

#include "SDL.h"
#include "Interface.hpp"

class EventManager{
public:
	EventManager(Interface &interface);
	~EventManager();

	int ProcessEvents();
private:
	SDL_Event event;
	Interface &interface;
};
