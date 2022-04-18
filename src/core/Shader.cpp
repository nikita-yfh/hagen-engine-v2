#include "Shader.hpp"
#include "Logger.hpp"

Shader::Shader(const char *fragSource, const char *vertSource) {
	GLuint vert = -1;
	GLuint frag = -1;
	if(fragSource) {
		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &fragSource, nullptr);
		glCompileShader(frag);
		CheckCompileErrors(frag);
	}
	if(vertSource) {
		vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, &vertSource, nullptr);
		glCompileShader(vert);
		CheckCompileErrors(vert);
	}
	id = glCreateProgram();
	if(fragSource)
		glAttachShader(id, frag);
	if(vertSource)
		glAttachShader(id, vert);
	glLinkProgram(id);
	CheckLinkErrors();
	if(fragSource)
		glDeleteShader(frag);
	if(vertSource)
		glDeleteShader(vert);
}
Shader::~Shader() {
	glDeleteProgram(id);
}
bool Shader::CheckCompileErrors(GLuint shader) const {
	int success;
	char log[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 1024, nullptr, log);
		Log(LEVEL_WARNING, log);
		return false;
	}
	return true;
}
bool Shader::CheckLinkErrors() const {
	int success;
	char log[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(id, 1024, nullptr, log);
		Log(LEVEL_WARNING, log);
		return false;
	}
	return true;
}
void Shader::SetUniform(const char *name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name), value);
}
void Shader::SetUniform(const char *name, int value) const {
	glUniform1i(glGetUniformLocation(id, name), value);
}
void Shader::SetUniform(const char *name, unsigned value) const {
	glUniform1ui(glGetUniformLocation(id, name), value);
}
void Shader::SetUniform(const char *name, float value) const {
	glUniform1f(glGetUniformLocation(id, name), value);
}
void Shader::SetUniform(const char *name, const b2Vec2 &value) const {
	glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}
void Shader::SetUniform(const char *name, Texture *texture) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());
    glUniform1i(glGetUniformLocation(id, name), 0);
}

