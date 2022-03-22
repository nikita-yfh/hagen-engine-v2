#pragma once
#include "Joint.hpp"
#include "Rotatable.hpp"

class PrismaticJoint : public Joint, public Rotatable{
public:
	PrismaticJoint();

	virtual void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;

	virtual bool Create(const Mouse &mouse) override;
	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	bool enableLimit;
	float lowerTranslation;
	float upperTranslation;

	bool enableMotor;
	float maxMotorForce;
	float motorSpeed;
};


