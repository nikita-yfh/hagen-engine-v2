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

class Poly : public Fixture {
public:
	Poly() : points(nullptr) {}
	virtual ~Poly();

	void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	virtual void Reverse() override;
	virtual bool TestPoint(const b2Vec2 &point) const override;
	virtual bool CancelCreating() override;

	void AddPoint(b2Vec2 point);
	void ClearPoints();
	void SavePoints(rapidjson::Value &value, jsonutils::Allocator &allocator) const;

	virtual void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const override;
	virtual bool FromJSON(const rapidjson::Value &value) override;
protected:
	int GetPointCount() const;
	Point *points;
};
