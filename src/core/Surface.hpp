#pragma once

#include "SDL.h"
#include "Resource.hpp"

struct Surface : Resource{
	Surface();
	~Surface();

	int Load(SDL_RWops *file);
	int Save(SDL_RWops *file) const;

	SDL_Surface *surface;
};
