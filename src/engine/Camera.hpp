#pragma once

#include "b2_math.h"

class Camera{
public:
	Camera();

	void Apply() const;
private:
	b2Vec2 position;
	float zoom;
};
