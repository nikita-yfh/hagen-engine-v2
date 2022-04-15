#pragma once

#include "SDL.h"
#include "document.h"

struct Resource {
public:
	virtual ~Resource() {}
	virtual bool Save(SDL_RWops *file) const { return false; }
	virtual bool Load(SDL_RWops *file) = 0;
};
struct TextResource : Resource {
public:
	TextResource();
	~TextResource();

	virtual bool Save(SDL_RWops *file) const override;
	virtual bool Load(SDL_RWops *file) override;

	bool Load(const char *text);

	char *data;
	size_t size;
private:
	bool AllocateBuffer();
};
struct JSONResource : Resource{
public:
	JSONResource();

	virtual bool Save(SDL_RWops *file) const override;
	virtual bool Load(SDL_RWops *file) override;

	bool Load(const char *text, size_t size);
	bool Load(const char *text);

	rapidjson::Document document;
private:
	bool LogError(const char *text) const;
};
