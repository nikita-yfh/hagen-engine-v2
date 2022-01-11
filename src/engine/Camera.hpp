#pragma once

#include "b2_math.h"

class Camera{
public:
	Camera();
	void SetBounds(const b2Vec2& lower, const b2Vec2 &upper);

	void Apply() const;

private:
	b2Vec2 position;

	b2Vec2 lowerBounds;
	b2Vec2 upperBounds;

};
