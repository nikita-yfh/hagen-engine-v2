#pragma once
#include "Colors.hpp"
#include "Object.hpp"
#include "b2_fixture.h"

class Body;

class Fixture : public Object{
public:
	Fixture();
	void SetParent(Body *parent);

	virtual uint8_t GetObjectType() const override{return FIXTURE;}
	virtual bool TryRemove(const void *object) override;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
	virtual bool TestPoint(const b2Vec2 &point) const = 0;

	virtual void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	virtual bool Load(const rapidjson::Value &value);
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
};

