#pragma once

#include <stdint.h>
#include "Texture.hpp"
#include "b2_world.h"

class Image {
public:
	Image *GetNext();
	const Image *GetNext() const;

	const b2Vec2& GetPosition() const;
	float GetAngle() const;

private:
	Image() {};

	uint32_t hash;

	b2Vec2 position;
	float angle;
	int8_t layer;
	b2Vec2 repeat;

	const Texture *texture;
	const b2Body *bindBody;

	Image *prev;
	Image *next;

	friend class Level;
};

inline Image *Image::GetNext() {
	return next;
}
inline const Image *Image::GetNext() const {
	return next;
}
inline const b2Vec2 &Image::GetPosition() const {
	return position;
}
inline float Image::GetAngle() const {
	return angle;
}
