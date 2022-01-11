#pragma once
#include "PointJoint.hpp"

class PrismaticJoint : public Joint{
public:
	PrismaticJoint();

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	virtual void DrawJoint(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;

	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	void Transform() const;
	void TransformBack() const;

	Mouse GetLocalMouse(const Mouse &mouse) const;

	b2Vec2 position;
	float angle;

	bool enableLimit;
	float lowerTranslation;
	float upperTranslation;

	bool enableMotor;
	float maxMotorForce;
	float motorSpeed;
};


