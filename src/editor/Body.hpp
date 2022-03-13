#pragma once
#include "b2_body.h"
#include "Fixture.hpp"
#include "Colors.hpp"
#include "Object.hpp"

class Level;

class Body : public Object{
public:
	Body(b2BodyType type);

	virtual uint8_t GetType() const override{return BODY;}

	virtual void Draw(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	Color GetColor() const;
	float GetLineWidth() const;

	Mouse GetLocalMouse(const Mouse &mouse) const;

	const b2Vec2 &GetPosition() const;
	float GetAngle() const;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	void Transform() const;
	void TransformBack() const;

	b2BodyType type;
	b2Vec2 position;
	float angle;
	b2Vec2 linearVelocity;
	float angularVelocity;
	float linearDamping;
	float angularDamping;
	bool allowSleep;
	bool awake;
	bool fixedRotation;
	bool bullet;
	bool enabled;
	float gravityScale;

	Fixture *fixtures;

	friend class Level;
};
