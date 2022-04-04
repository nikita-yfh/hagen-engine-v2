#pragma once
#include "PointJoint.hpp"

class WeldJoint : public PointJoint{
public:
	WeldJoint();

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;

	virtual void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const override;
	virtual bool Load(const rapidjson::Value &value) override;
private:
	float stiffness;
	float damping;
};
