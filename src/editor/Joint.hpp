#pragma once
#include "Object.hpp"
#include "Body.hpp"

class Level;

class Joint : public Object{
public:
	Joint();

	void SetBodies(Body *a, Body *b);

	bool HasBody(const Body *body) const;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
protected:
	void Draw(const Colors &colors) const;
	virtual void DrawJoint(const Colors &colors) const = 0;

	virtual void DrawPoints(const Colors &colors) const = 0;
	virtual bool UpdatePoints(const Mouse &mouse) = 0;

	void DrawConnections(const Colors &colors, const b2Vec2 &point) const;
	void DrawConnections(const Colors &colors, const b2Vec2 &p1, const b2Vec2 &p2) const;

	Mouse GetLocalMouseA(const Mouse &mouse);
	Mouse GetLocalMouseB(const Mouse &mouse);

	Body *a;
	Body *b;
	bool collideConnected;

	Joint *next;

	friend class Level;
};


