#pragma once

#include "SDL.h"
#include "Resource.hpp"

struct Surface : Resource{
	Surface();
	~Surface();

	bool Load(SDL_RWops *file);

	SDL_Surface *surface;
};
