#pragma once

#include "SDL.h"
#include "Resource.hpp"

struct TextEntry{
	const char *str;
	const char *index;
	TextEntry *next;
};

class Locale : public JSONResource{
public:
	Locale(SDL_RWops *file);
	~Locale();

	void Clear();

	const char *operator[](const char *index);
private:

};
