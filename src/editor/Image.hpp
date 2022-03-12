#pragma once
#include "Object.hpp"
#include "Texture.hpp"
#include "Body.hpp"

class Level;

class Image : public Object {
public:
	Image(const Texture *texture, const float &scale);

	void Draw(const Colors &colors) const;

	void DrawPoints(const Colors &colors) const;
	bool UpdatePoints(const Mouse &mouse);

	virtual bool Create(const Mouse &mouse) override;

private:
	bool TestPoint(const b2Vec2 &point) const;

	b2Vec2 position;
	float angle;
	const Texture *texture;
	const Body *bindBody;

	const float &scale;
	Image *next;

	friend class Level;
};



