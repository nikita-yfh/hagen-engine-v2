#include "GLUtils.hpp"

namespace glutils{

static const float circleQuality=60;

void DrawCircle(const b2Vec2 &pos, float radius, bool fill) {
	if(fill){
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(pos.x,pos.y);
	}else
		glBegin(GL_LINE_LOOP);
	for(float i = 0; i < M_PI*2.0f; i+=M_PI*2.0f/circleQuality) {
		b2Vec2 rot(cosf(i),sinf(i));
		rot*=radius;
		Vertex(pos+rot);
	}
	glEnd();
}

void DrawLine(const b2Vec2 &p1, const b2Vec2 &p2){
	glBegin(GL_LINES);
	Vertex(p1);
	Vertex(p2);
	glEnd();
}

float VecToAngle(const b2Vec2 &p) {
	if(p.x>=0)
		return M_PI+atan(p.y/p.x);
	else if(p.y>0)
		return 2*M_PI+atan(p.y/p.x);
	else
		return atan(p.y/p.x);
}
float VecToAngle2(const b2Vec2 &p) {
	return fmod(M_PI+VecToAngle(p),M_PI*2);
}

};
