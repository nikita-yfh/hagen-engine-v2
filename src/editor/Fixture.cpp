#include "Fixture.hpp"
#include "GLUtils.hpp"
#include "Body.hpp"
#include <wx/glcanvas.h>

Fixture::Fixture():
	friction(0.3f),
	restitution(0.0f),
	restitutionThreshold(1.0f * b2_lengthUnitsPerMeter),
	density(0.0f),
	isSensor(false),
	parent(nullptr) {}

void Fixture::SetParent(Body *_parent){
	parent = _parent;
}
bool Fixture::TryRemove(const void *object){
	if(object == parent)
		return false;
	return true;
}
void Fixture::ApplyFill(const Colors &colors) const{
	colors.Apply(parent->GetColor(),fillAlpha);
}
void Fixture::ApplyBorder(const Colors &colors) const{
	colors.Apply(parent->GetColor(),borderAlpha);
}
bool Fixture::UpdateBody(const Mouse &mouse) const{
	if(mouse.pressed && TestPoint(mouse.position)){
		parent->BeginDrag(mouse);
		return true;
	}
	return false;
}

bool Fixture::CanBeDynamic() const{
	return true;
}

void Fixture::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n){
		pg->GetProperty("Friction")->SetValue(friction);
		pg->GetProperty("Restitution")->SetValue(restitution);
		pg->GetProperty("RestitutionThresold")->SetValue(restitutionThreshold);
		pg->GetProperty("Density")->SetValue(density);
		pg->GetProperty("IsSensor")->SetValue(isSensor);
	}else{
		pg->Append(new wxFloatProperty("Friction", wxPG_LABEL, friction));
		pg->Append(new wxFloatProperty("Restitution", wxPG_LABEL, restitution));
		pg->Append(new wxFloatProperty("RestitutionThresold", wxPG_LABEL, restitutionThreshold));
		pg->Append(new wxFloatProperty("Density", wxPG_LABEL, density));
		pg->Append(new wxBoolProperty("IsSensor", wxPG_LABEL, isSensor));
	}
	Object::UpdatePropertyGrid(pg,n);
}
void Fixture::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name=="Friction")
		friction = value.GetDouble();
	else if(name=="Restitution")
		restitution = value.GetDouble();
	else if(name=="RestitutionThresold")
		restitutionThreshold = value.GetDouble();
	else if(name=="Density")
		density = value.GetDouble();
	else if(name=="IsSensor")
		isSensor = value.GetBool();
	Object::OnPropertyGridChange(name, value);
}



