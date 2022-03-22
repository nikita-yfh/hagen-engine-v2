#include "Texture.hpp"
#include "EditorApp.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const Directory &dir, const wxString &_name) : name(_name) {
	wxString path = dir.ConvertToAbsolutePath(name);
	glGenTextures(1, &texture);
	Bind();
	LoadTexture(path);
}
Texture::~Texture(){
	glDeleteTextures(1, &texture);
}
bool Texture::LoadTexture(const wxString &path) {
	int channels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if(data){
		SetFiltering(GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		return true;
	}
	width = 0;
	height = 0;
	return false;
}
bool Texture::Reload(const Directory &dir) {
	wxString path = dir.ConvertToAbsolutePath(name);
	return LoadTexture(path);
}
void Texture::Bind() const{
	glBindTexture(GL_TEXTURE_2D,texture);
}
void Texture::Activate() const{
	Bind();
	glEnable(GL_TEXTURE_2D);
}
void Texture::Deactivate() const{
	glDisable(GL_TEXTURE_2D);
}
GLuint Texture::GetID() const{
	return texture;
}
void Texture::SetParameterf(GLenum pname,float value) const{
	Bind();
	glTexParameterf(GL_TEXTURE_2D,pname,value);
}
void Texture::SetParameteri(GLenum pname,int value) const{
	Bind();
	glTexParameteri(GL_TEXTURE_2D,pname,value);
}
float Texture::GetParameterf(GLenum pname) const{
	Bind();
	float value;
	glGetTexParameterfv(GL_TEXTURE_2D,pname,&value);
	return value;
}

int Texture::GetParameteri(GLenum pname) const{
	Bind();
	int value;
	glGetTexParameteriv(GL_TEXTURE_2D,pname,&value);
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
int Texture::GetHeight() const{
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
