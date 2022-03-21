#pragma once
#include "Colors.hpp"
#include "Object.hpp"
#include "b2_fixture.h"

class Body;
class Level;

class Fixture : public Object{
public:
	Fixture();

	virtual uint8_t GetType() const override{return FIXTURE;}
	virtual bool TryRemove(const void *object) override;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
	virtual bool TestPoint(const b2Vec2 &point) const = 0;
protected:
	void ApplyFill(const Colors &colors) const;
	void ApplyBorder(const Colors &colors) const;
	bool UpdateBody(const Mouse &mouse) const;

	virtual bool CanBeDynamic() const;

	float friction;
	float restitution;
	float restitutionThreshold;
	float density;
	bool isSensor;
	b2Filter filter;

	Body *parent;

	friend class Level;
};

