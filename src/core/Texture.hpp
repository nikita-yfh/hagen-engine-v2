#pragma once

#include "SDL.h"
#include "glad/glad.h"
#include "Resource.hpp"

class Texture : public Resource {
public:
	Texture();
	~Texture();

	virtual bool Load(SDL_RWops *file) override;

	void Bind() const;
	void Activate() const;
	void Deactivate() const;

	GLuint GetID() const;

	int GetWidth() const;
	int GetHeigth() const;

	void SetWrapX(GLenum wrap);
	void SetWrapY(GLenum wrap);
	GLenum GetWrapX()const;
	GLenum GetWrapY()const;

	void SetMagFiltering(GLenum type);
	void SetMinFiltering(GLenum type);
	void SetFiltering(GLenum type);
	GLenum GetMagFiltering() const;
	GLenum GetMinFiltering() const;

private:
	GLuint texture;
	int width;
	int height;

	void	SetParameteri(GLenum pname, int value) const;
	void	SetParameterf(GLenum pname, float value) const;

	int		GetParameteri(GLenum pname) const;
	float	GetParameterf(GLenum pname) const;
};
