#pragma once
#include "Joint.hpp"

class PulleyJoint : public Joint{
public:
	PulleyJoint();

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	virtual void DrawJoint(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;

	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	b2Vec2 groundA;
	b2Vec2 groundB;

	b2Vec2 localA;
	b2Vec2 localB;

	float ratio;
};


