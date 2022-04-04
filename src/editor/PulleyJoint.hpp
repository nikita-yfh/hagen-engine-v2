#pragma once
#include "Joint.hpp"

class PulleyJoint : public Joint{
public:
	PulleyJoint();

	virtual void Draw(const Colors &colors) const override;

	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;

	virtual void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const override;
	virtual bool Load(const rapidjson::Value &value) override;
private:
	b2Vec2 groundA;
	b2Vec2 groundB;

	b2Vec2 localA;
	b2Vec2 localB;

	float ratio;
};


