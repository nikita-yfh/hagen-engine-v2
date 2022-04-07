#include "PulleyJoint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

PulleyJoint::PulleyJoint()
	:groundA(b2Vec2_zero), groundB(b2Vec2_zero),
	localA(b2Vec2_zero), localB(b2Vec2_zero), ratio(1.0f) {selected=1;}

void PulleyJoint::Draw(const Colors &colors) const{
	ApplyDrawJoint(colors);
	glBegin(GL_LINE_STRIP);
	glutils::Vertex(a->GetPosition()+localA);
	glutils::Vertex(groundA);
	glutils::Vertex(groundB);
	glutils::Vertex(b->GetPosition()+localB);
	glEnd();
	DrawConnections(colors,localA,localB);
}
void PulleyJoint::DrawPoints(const Colors &colors) const{
	DrawPoint(colors,1,a->GetPosition()+localA);
	DrawPoint(colors,2,groundA);
	DrawPoint(colors,3,groundB);
	DrawPoint(colors,4,b->GetPosition()+localB);
}
bool PulleyJoint::UpdatePoints(const Mouse &mouse){
	return
		UpdatePoint(GetLocalMouseA(mouse),1,localA) ||
		UpdatePoint(mouse,2,groundA) ||
		UpdatePoint(mouse,3,groundB) ||
		UpdatePoint(GetLocalMouseB(mouse),4,localB);
}
bool PulleyJoint::Create(const Mouse &mouse){
	UpdatePoints(mouse);
	if(selected<2)
		groundA=groundB=b->GetPosition();
	if(mouse.pressed){
		if(selected<4)
			selected++;
		else
			return true;
	}
	return false;
}
void PulleyJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n){
		pg->GetProperty("groundA")->SetValue(WXVARIANT(groundA));
		pg->GetProperty("groundB")->SetValue(WXVARIANT(groundB));
		pg->GetProperty("localA")->SetValue(WXVARIANT(localA));
		pg->GetProperty("localB")->SetValue(WXVARIANT(localB));
	}else{
		pg->Append(new Vec2Property("groundA", wxPG_LABEL, groundA));
		pg->Append(new Vec2Property("groundB", wxPG_LABEL, groundB));
		pg->Append(new Vec2Property("localB", wxPG_LABEL, localA));
		pg->Append(new Vec2Property("localA", wxPG_LABEL, localB));
		pg->Append(new wxFloatProperty("ratio", wxPG_LABEL, ratio));
	}
	Joint::UpdatePropertyGrid(pg,n);
}
void PulleyJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "groundA")
		groundA << value;
	else if(name == "groundB")
		groundB << value;
	else if(name == "localA")
		localA << value;
	else if(name == "localB")
		localB << value;
	else if(name == "ratio")
		ratio = value.GetDouble();
	Joint::OnPropertyGridChange(name,value);
}
void PulleyJoint::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "pulley", allocator);
	value.AddMember("groundA", jsonutils::Value(groundA, allocator), allocator);
	value.AddMember("groundB", jsonutils::Value(groundB, allocator), allocator);
	value.AddMember("localA", jsonutils::Value(localA, allocator), allocator);
	value.AddMember("localB", jsonutils::Value(localB, allocator), allocator);
	value.AddMember("ratio", ratio, allocator);
	Joint::ToJSON(value, allocator);
}
bool PulleyJoint::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "groundA", groundA) &&
		jsonutils::GetMember(value, "groundB", groundB) &&
		jsonutils::GetMember(value, "localA", localA) &&
		jsonutils::GetMember(value, "localB", localB) &&
		jsonutils::GetMember(value, "ratio", ratio) &&
		Joint::FromJSON(value);
}


