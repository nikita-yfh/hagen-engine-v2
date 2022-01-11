#pragma once
#include "Colors.hpp"
#include "Object.hpp"
#include "b2_fixture.h"

class Body;

class Fixture : public Object{
public:
	Fixture();

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
	virtual bool TestPoint(const b2Vec2 &point) const = 0;
protected:
	void ApplyFill(const Colors &colors) const;
	void ApplyBorder(const Colors &colors) const;

	virtual void Draw(const Colors &colors) const = 0;

	virtual void DrawPoints(const Colors &colors) const = 0;
	virtual bool UpdatePoints(const Mouse &mouse) = 0;
	virtual bool Create(const Mouse &mouse);
	virtual bool CreateFixture(const Mouse &mouse) = 0;

	virtual bool CanBeDynamic() const;

	float friction;
	float restitution;
	float restitutionThreshold;
	float density;
	bool isSensor;
	b2Filter filter;

	Fixture *next;
	Body *parent;

	friend class Body;
};

