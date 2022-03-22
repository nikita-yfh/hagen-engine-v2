#pragma once
#include "b2_math.h"
#include "Colors.hpp"
#include "Object.hpp"
#include "GLUtils.hpp"

class RotatableObject : public Object{
public:
	RotatableObject();

	Mouse GetLocalMouse(const Mouse &mouse) const;

	const b2Vec2 &GetPosition() const;
	float GetAngle() const;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;

	void Transform() const;
	inline void TransformBack() const{
		glPopMatrix();
	}
protected:
	void SetAngle(float a);

	b2Vec2 position;
	float angle;

	b2Rot rotate;
};
