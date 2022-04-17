#pragma once

#include <stdint.h>
#include "Texture.hpp"
#include "String.hpp"
#include "b2_body.h"

class Image {
public:
	Image *GetNext();
	const Image *GetNext() const;

	const b2Vec2& GetPosition() const;
	float GetAngle() const;

	int8_t GetLayer() const;
	void SetLayer(int8_t layer);

	void Render(float textureScale) const;
private:

	Image() {};

	hash_t hash;

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
inline int8_t Image::GetLayer() const {
	return layer;
}
inline void Image::SetLayer(int8_t _layer) {
	layer = _layer;
}
