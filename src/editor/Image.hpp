#pragma once
#include "Object.hpp"
#include "Texture.hpp"
#include "Body.hpp"

class Level;

class Image : public Object {
public:
	Image(const Texture *texture, const float &scale);

	virtual void Draw(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;
	virtual uint8_t GetType() const override{return IMAGE;}

	virtual bool TryRemove(const void *object);
private:
	bool TestPoint(const b2Vec2 &point) const;

	b2Vec2 position;
	float angle;
	const Texture *texture;
	const Body *bindBody;

	const float &scale;

	friend class Level;
};



