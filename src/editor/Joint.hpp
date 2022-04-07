#pragma once
#include "Object.hpp"
#include "Body.hpp"

class Joint : public Object{
public:
	Joint();
	void SetBodies(Body *a, Body *b);

	virtual bool TryRemove(const void *object) override;
	virtual uint8_t GetObjectType() const override{return JOINT;}

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;

	virtual void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	virtual bool FromJSON(const rapidjson::Value &value);
protected:
	void ApplyDrawJoint(const Colors &colors) const;

	void DrawConnections(const Colors &colors, const b2Vec2 &point) const;
	void DrawConnections(const Colors &colors, const b2Vec2 &p1, const b2Vec2 &p2) const;

	Mouse GetLocalMouseA(const Mouse &mouse);
	Mouse GetLocalMouseB(const Mouse &mouse);

	Body *a;
	Body *b;
	bool collideConnected;
};


