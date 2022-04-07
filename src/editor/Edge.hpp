#pragma once
#include "Fixture.hpp"

class Edge : public Fixture{
public:
	Edge();

	void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	virtual void Reverse() override;
	virtual bool TestPoint(const b2Vec2 &point) const override;
	virtual bool CanBeDynamic() const override;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;

	virtual void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const override;
	virtual bool FromJSON(const rapidjson::Value &value) override;
private:
	b2Vec2 p1;
	b2Vec2 p2;
};
