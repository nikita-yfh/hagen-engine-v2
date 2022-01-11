#include "Texture.hpp"

#include "Surface.hpp"

Texture::Texture()
	:texture(0) {}

Texture::~Texture() {
	glDeleteTextures(1,&texture);
}
int Texture::Load(SDL_RWops *file){
	Surface srcRes;

	if(srcRes.Load(file))
		return 1;

	SDL_Surface *&surface=srcRes.surface;

	glGenTextures(1,&texture);

	width=surface->w;
	height=surface->h;
	Bind();
	glTexImage2D(type, 1, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return 0;
}
void Texture::Bind() const{
	glBindTexture(type,texture);
}
void Texture::Activate() const{
	Bind();
	glEnable(type);
}
void Texture::Deactivate() const{
	glDisable(type);
}
GLuint Texture::GetID() const{
	return texture;
}
void Texture::SetParameterf(GLenum pname,float value) const{
	Bind();
	glTexParameterf(type,pname,value);
}
void Texture::SetParameteri(GLenum pname,int value) const{
	Bind();
	glTexParameteri(type,pname,value);
}
float Texture::GetParameterf(GLenum pname) const{
	Bind();
	float value;
	glGetTexParameterfv(type,pname,&value);
	return value;
}

int Texture::GetParameteri(GLenum pname) const{
	Bind();
	int value;
	glGetTexParameteriv(type,pname,&value);
	return value;
}
void Texture::SetWrapX(GLenum wrap){
	SetParameteri(GL_TEXTURE_WRAP_S,wrap);
}
void Texture::SetWrapY(GLenum wrap){
	SetParameteri(GL_TEXTURE_WRAP_T,wrap);
}
GLenum Texture::GetWrapX() const{
	return GetParameteri(GL_TEXTURE_WRAP_S);
}
GLenum Texture::GetWrapY() const{
	return GetParameteri(GL_TEXTURE_WRAP_T);
}
int Texture::GetWidth() const{
	return width;
}
int Texture::GetHeigth() const{
	return height;
}
void Texture::SetMagFiltering(GLenum type){
	SetParameteri(GL_TEXTURE_MAG_FILTER,type);
}
void Texture::SetMinFiltering(GLenum type){
	SetParameteri(GL_TEXTURE_MIN_FILTER,type);
}
void Texture::SetFiltering(GLenum type){
	SetMinFiltering(type);
	SetMagFiltering(type);
}
GLenum Texture::GetMagFiltering() const{
	return GetParameteri(GL_TEXTURE_MAG_FILTER);
}
GLenum Texture::GetMinFiltering() const{
	return GetParameteri(GL_TEXTURE_MIN_FILTER);
}
