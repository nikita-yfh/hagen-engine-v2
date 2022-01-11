#pragma once
#include "b2_body.h"
#include "Fixture.hpp"
#include "Colors.hpp"
#include "Object.hpp"

class Level;

class Body : public Object{
public:
	Body(b2BodyType type);
	~Body();

	void Draw(const Colors &colors) const;

	void DrawPoints(const Colors &colors) const;
	bool UpdatePoints(const Mouse &mouse);
	bool Create(const Mouse &mouse);

	void ClearFixtures();
	void AddFixture(Fixture *fixture);

	Color GetColor() const;
	float GetLineWidth() const;

	void UnselectPoints();
	void SelectAll();
	void UnselectAll();
	int GetSelectedCount() const;
	void DeleteSelected();
	Object *GetFirstSelected();

	Mouse GetLocalMouse(const Mouse &mouse) const;

	void SetType(b2BodyType type);

	const b2Vec2 &GetPosition() const;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	bool TestPoint(const b2Vec2 &pos) const;

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
	Body *next;

	friend class Level;
};
