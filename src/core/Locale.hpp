#pragma once

#include "SDL.h"
#include "Resource.hpp"
#include "String.hpp"
#include "JSONUtils.hpp"

struct TextEntry;

class Locale : public Resource{
public:
	Locale();
	~Locale();

	virtual bool Load(SDL_RWops *file) override;

	void Clear();

	const char *operator[](const char *index) const;
private:
	bool ProcessJSONValue(const char *prefix, const rapidjson::Value &value);
	TextEntry *entries;
};
