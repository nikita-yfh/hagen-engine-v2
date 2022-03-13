#pragma once
#include "Fixture.hpp"

struct Point : public b2Vec2{
	Point(float _x,float _y)
		:b2Vec2(_x,_y),next(nullptr) {}
	Point(b2Vec2 point)
		:b2Vec2(point),next(nullptr) {}
	Point()
		:b2Vec2(), next(nullptr) {}
	Point *next;
};

class Polygon : public Fixture{
public:
	Polygon();

	void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	virtual bool TestPoint(const b2Vec2 &point) const override;
	virtual bool CancelCreating() override;
protected:
	int GetPointCount() const;
	Point *points;
};
