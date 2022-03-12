#pragma once
#include "Directory.hpp"
#include <wx/glcanvas.h>

class Level;

class Texture {
public:
	Texture(const Directory &dir, const wxString &name);
	~Texture();

	void Bind() const;
	void Activate() const;
	void Deactivate() const;

	GLuint GetID() const;

	int GetWidth() const;
	int GetHeight() const;

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
	wxString name;

	void	SetParameteri(GLenum pname,int value) const;
	void	SetParameterf(GLenum pname,float value) const;

	int		GetParameteri(GLenum pname) const;
	float	GetParameterf(GLenum pname) const;

	void operator=(const Texture&) = delete;
	Texture(const Texture&) = delete;

	Texture *next;

	friend class Level;
};
