#pragma once
#include "Fixture.hpp"
#include "Poly.hpp"

class Chain : public Poly{
public:
	virtual void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;

	virtual bool TestPoint(const b2Vec2 &point) const override;
	virtual bool CanBeDynamic() const override;

	virtual void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const override;
};
