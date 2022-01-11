#pragma once
#include "b2_math.h"

namespace glutils{

void DrawCircle(const b2Vec2 &pos, float radius, bool fill);
void DrawLine(const b2Vec2 &p1, const b2Vec2 &p);
void Vertex(const b2Vec2 &p);
void Translate(const b2Vec2 &p);
void Rotate(float rad);

float RadToDeg(float rad);
float DegToRad(float deg);

float VecToAngle(const b2Vec2 &p);
float VecToAngle2(const b2Vec2 &p);

};
