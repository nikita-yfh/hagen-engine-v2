#pragma once
#include "Camera.hpp"
#include "b2_math.h"

class Mouse{
public:
	Mouse(const b2Vec2 &_pos, bool _pressed, const Camera &_camera)
		:position(_pos), pressed(_pressed), camera(_camera) {}

	Mouse operator+(const b2Vec2 &vec) const{
		return Mouse(position-vec,pressed,camera);
	}
	Mouse operator*(const b2Transform &transform) const{
		return Mouse(b2MulT(transform,position),pressed,camera);
	}
	Mouse operator-(const b2Vec2 &vec) const{
		return Mouse(position+vec,pressed,camera);
	}
	Mouse operator/(const b2Transform &transform) const{
		return Mouse(b2Mul(transform,position),pressed,camera);
	}

	b2Vec2 position;
	float zoom;
	bool pressed;
	const Camera &camera;
};

