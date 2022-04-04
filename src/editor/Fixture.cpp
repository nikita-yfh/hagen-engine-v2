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
		pg->GetProperty("friction")->SetValue(friction);
		pg->GetProperty("restitution")->SetValue(restitution);
		pg->GetProperty("restitutionThresold")->SetValue(restitutionThreshold);
		pg->GetProperty("density")->SetValue(density);
		pg->GetProperty("isSensor")->SetValue(isSensor);
	}else{
		pg->Append(new wxFloatProperty("friction", wxPG_LABEL, friction));
		pg->Append(new wxFloatProperty("restitution", wxPG_LABEL, restitution));
		pg->Append(new wxFloatProperty("restitutionThresold", wxPG_LABEL, restitutionThreshold));
		pg->Append(new wxFloatProperty("density", wxPG_LABEL, density));
		pg->Append(new wxBoolProperty("isSensor", wxPG_LABEL, isSensor));
	}
	Object::UpdatePropertyGrid(pg,n);
}
void Fixture::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name=="friction")
		friction = value.GetDouble();
	else if(name=="restitution")
		restitution = value.GetDouble();
	else if(name=="restitutionThresold")
		restitutionThreshold = value.GetDouble();
	else if(name=="density")
		density = value.GetDouble();
	else if(name=="isSensor")
		isSensor = value.GetBool();
	Object::OnPropertyGridChange(name, value);
}
void Fixture::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("friction", friction, allocator);
	value.AddMember("restitution", restitution, allocator);
	value.AddMember("restitutionThreshold", restitutionThreshold, allocator);
	value.AddMember("density", density, allocator);
	value.AddMember("isSensor", isSensor, allocator);
	Object::Save(value, allocator);
}
bool Fixture::Load(const rapidjson::Value &value){
	return 
		jsonutils::GetMember(value, "friction", friction) ||
		jsonutils::GetMember(value, "restitution", restitution) ||
		jsonutils::GetMember(value, "restitutionThreshold", restitutionThreshold) ||
		jsonutils::GetMember(value, "density", density) ||
		jsonutils::GetMember(value, "isSensor", isSensor) ||
		Object::Load(value);
}
