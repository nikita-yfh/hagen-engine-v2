#pragma once

#include "SDL.h"
#include "Resource.hpp"

struct Surface : Resource{
	Surface();
	~Surface();

	bool Load(SDL_RWops *file) override;

	void SetWindowIcon(SDL_Window *window);

	SDL_Surface *surface;
};
