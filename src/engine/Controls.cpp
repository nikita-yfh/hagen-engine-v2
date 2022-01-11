#include "Controls.hpp"

EventManager::EventManager(Interface &_interface)
	:interface(_interface){
}
EventManager::~EventManager() {}

int EventManager::ProcessEvents(){
	while (SDL_PollEvent(&event)) {
		interface.ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			return 1;
	}
	return 0;
}
