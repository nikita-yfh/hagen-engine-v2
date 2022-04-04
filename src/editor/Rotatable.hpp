#pragma once
#include "b2_math.h"
#include "Colors.hpp"
#include "Object.hpp"
#include <wx/propgrid/propgrid.h>

class Rotatable{
public:
	Rotatable();

	Mouse GetLocalMouse(const Mouse &mouse) const;

	const b2Vec2 &GetPosition() const;
	float GetAngle() const;

	void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const;
	void OnPropertyGridChange(const wxString &name, const wxVariant &value);

	void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool Load(const rapidjson::Value &value);

	void Transform() const;
protected:
	void SetAngle(float a);

	b2Vec2 position;
	float angle;

	b2Rot rotate;
};
