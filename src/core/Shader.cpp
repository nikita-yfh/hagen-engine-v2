#include "Shader.hpp"

Shader::Shader(GLenum type) {
	id = glCreateShader(type);
}
Shader::~Shader() {
	glDeleteShader(id);
}

virtual bool Load(SDL_RWops *file) {
	TextResource textRes;
	if(!textRes.Load(file))
		return false;
	return Load(textRes.data, textRes.size);
