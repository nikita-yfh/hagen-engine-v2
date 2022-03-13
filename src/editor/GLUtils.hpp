#pragma once
#include "b2_math.h"
#include <wx/glcanvas.h>
#include <math.h>

namespace glutils{

void DrawCircle(const b2Vec2 &pos, float radius, bool fill);
void DrawLine(const b2Vec2 &p1, const b2Vec2 &p);

float VecToAngle(const b2Vec2 &p);
float VecToAngle2(const b2Vec2 &p);

inline float RadToDeg(float rad){
	return rad/M_PI*180.0f;
}
inline float DegToRad(float deg){
	return deg/180.0f*M_PI;
}
inline void Vertex(const b2Vec2 &p){
	glVertex2f(p.x,p.y);
}
inline void Translate(const b2Vec2 &p){
	glTranslatef(p.x,p.y,0.0f);
}
inline void Rotate(float rad){
	glRotatef(RadToDeg(rad),0.0f,0.0f,1.0f);
}

};
