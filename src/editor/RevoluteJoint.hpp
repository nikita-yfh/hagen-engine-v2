#pragma once
#include "PointJoint.hpp"

class RevoluteJoint : public PointJoint{
public:
	RevoluteJoint();

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	bool enableLimit;
	float lowerAngle;
	float upperAngle;
	bool enableMotor;
	float maxMotorTorque;
	float motorSpeed;
};

