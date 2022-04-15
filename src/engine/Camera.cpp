#include "Camera.hpp"
#include "glad.h"
#include "Logger.hpp"

Camera::Camera()
	: position(0.0f, 0.0f), zoom(100.0f) {}

void Camera::Apply() const{
    GLint v[4]; glGetIntegerv(GL_VIEWPORT, v);
	int width  = v[2] - v[0];
	int height = v[3] - v[1];
	glOrtho(-width/2, width/2, height/2, -height/2, 0.0f, 1.0f);
	glScalef(zoom, zoom, 1.0f);
	glTranslatef(-position.x, -position.y, 0.0f);
}
