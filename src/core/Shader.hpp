#pragma once

#include "b2_math.h"
#include "Texture.hpp"

class Shader {
public:
	Shader(const char *frag, const char *vert);
	~Shader();

	void Activate() const;
	static void Deactivate();

	void SetUniform(const char *name, bool value) const;
	void SetUniform(const char *name, unsigned value) const;
	void SetUniform(const char *name, int value) const;
	void SetUniform(const char *name, float value) const;
	void SetUniform(const char *name, const b2Vec2 &value) const;
	void SetUniform(const char *name, Texture *texture) const;

private:
	bool CheckCompileErrors(GLuint shader) const;
	bool CheckLinkErrors() const;
	GLuint id;
};

inline void Shader::Activate() const {
	glUseProgram(id);
}
inline void Shader::Deactivate() {
	glUseProgram(0);
}
