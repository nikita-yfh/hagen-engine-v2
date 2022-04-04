#pragma once
#include "Fixture.hpp"
#include "Polygon.hpp"

class Chain : public Polygon{
public:
	virtual void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;

	virtual bool TestPoint(const b2Vec2 &point) const override;
	virtual bool CanBeDynamic() const override;

	virtual void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const override;
};
