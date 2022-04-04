#include "DistanceJoint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

DistanceJoint::DistanceJoint()
	: localA(b2Vec2_zero), localB(b2Vec2_zero),max(0.0f),min(0.0f),
		stiffness(1000.0f),damping(0.5f) {}

void DistanceJoint::Draw(const Colors &colors) const{
	ApplyDrawJoint(colors);
	glutils::DrawLine(a->GetPosition()+localA,b->GetPosition()+localB);
	DrawConnections(colors,localA,localB);
}
void DistanceJoint::DrawPoints(const Colors &colors) const{
	DrawPoint(colors,1,a->GetPosition()+localA);
	DrawPoint(colors,2,b->GetPosition()+localB);
}
bool DistanceJoint::UpdatePoints(const Mouse &mouse){
	return
		UpdatePoint(GetLocalMouseA(mouse),1,localA) ||
		UpdatePoint(GetLocalMouseB(mouse),2,localB);
}
bool DistanceJoint::Create(const Mouse &mouse){
	UpdatePoints(mouse);
	if(mouse.pressed){
		if(selected==1)
			selected=2;
		else if(selected==2)
			return true;
	}
	return false;
}
void DistanceJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n){
		pg->GetProperty("localA")->SetValue(WXVARIANT(localA));
		pg->GetProperty("localB")->SetValue(WXVARIANT(localB));
	}else{
		pg->Append(new Vec2Property("localA", wxPG_LABEL, localA));
		pg->Append(new Vec2Property("localB", wxPG_LABEL, localB));
		pg->Append(new wxFloatProperty("min", wxPG_LABEL, min));
		pg->Append(new wxFloatProperty("max", wxPG_LABEL, max));
		pg->Append(new wxFloatProperty("stiffness", wxPG_LABEL, stiffness));
		pg->Append(new wxFloatProperty("damping", wxPG_LABEL, damping));
	}
	Joint::UpdatePropertyGrid(pg,n);
}
void DistanceJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "localA")
		localA << value;
	else if(name == "localB")
		localB << value;
	else if(name == "min")
		min = value.GetDouble();
	else if(name == "max")
		max = value.GetDouble();
	else if(name == "stiffness")
		stiffness = value.GetDouble();
	else if(name == "damping")
		damping = value.GetDouble();
	Joint::OnPropertyGridChange(name,value);
}
void DistanceJoint::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const {
	value.AddMember("type", "distance", allocator);
	value.AddMember("localA", jsonutils::Value(localA, allocator), allocator);
	value.AddMember("localB", jsonutils::Value(localB, allocator), allocator);
	value.AddMember("min", min, allocator);
	value.AddMember("max", max, allocator);
	value.AddMember("stiffness", stiffness, allocator);
	value.AddMember("damping", damping, allocator);
	Joint::Save(value, allocator);
}
bool DistanceJoint::Load(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "localA", localA) ||
		jsonutils::GetMember(value, "localB", localB) ||
		jsonutils::GetMember(value, "min", min) ||
		jsonutils::GetMember(value, "max", max) ||
		jsonutils::GetMember(value, "stiffness", stiffness) ||
		jsonutils::GetMember(value, "damping", damping) ||
		Joint::Load(value);
}
