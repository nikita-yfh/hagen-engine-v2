#pragma once

#include "SDL.h"
#include "document.h"

struct Resource {
	virtual ~Resource() {}
	virtual int Save(SDL_RWops *file) const = 0;
	virtual int Load(SDL_RWops *file) = 0;
};
struct TextResource : Resource {
	TextResource();
	~TextResource();

	virtual int Save(SDL_RWops *file) const;
	virtual int Load(const char *text);
	virtual int Load(SDL_RWops *file);

	int AllocateBuffer();
	char *data;
	size_t size;
};
struct JSONResource : Resource{
	JSONResource();

	virtual int Save(SDL_RWops *file) const;
	virtual int Load(SDL_RWops *file);
	virtual int Load(const char *text,size_t size);
	virtual int Load(const char *text);

	int LogError(const char *text) const;
	rapidjson::Document document;
};
