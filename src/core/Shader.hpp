#pragma once
#include "Resource.hpp"
#include "Texture.hpp"

class Shader : public Resource {
public:
	Shader(GLenum type);
	~Shader();

	virtual bool Load(SDL_RWops *file);
	bool Load(const char *text);

private:
	GLuint id;
};
