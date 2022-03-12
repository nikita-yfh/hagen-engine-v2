#pragma once
#include "Fixture.hpp"

class Circle : public Fixture{
public:
	Circle();

	void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool CreateFixture(const Mouse &mouse) override;

	virtual bool TestPoint(const b2Vec2 &point) const override;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:

	b2Vec2 position;
	float radius;
};