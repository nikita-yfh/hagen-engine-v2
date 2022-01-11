#include "DistanceJoint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

DistanceJoint::DistanceJoint()
	: localA(b2Vec2_zero), localB(b2Vec2_zero),max(0.0f),min(0.0f),
		stiffness(1000.0f),damping(0.5f) {selected=1;}

void DistanceJoint::DrawJoint(const Colors &colors) const{
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
		pg->GetProperty("LocalA")->SetValue(WXVARIANT(localA));
		pg->GetProperty("LocalB")->SetValue(WXVARIANT(localB));
	}else{
		pg->Append(new Vec2Property("LocalA", wxPG_LABEL, localA));
		pg->Append(new Vec2Property("LocalB", wxPG_LABEL, localB));
		pg->Append(new wxFloatProperty("Min", wxPG_LABEL, min));
		pg->Append(new wxFloatProperty("Max", wxPG_LABEL, max));
		pg->Append(new wxFloatProperty("Stiffness", wxPG_LABEL, stiffness));
		pg->Append(new wxFloatProperty("Damping", wxPG_LABEL, damping));
	}
	Joint::UpdatePropertyGrid(pg,n);
}
void DistanceJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "LocalA")
		localA << value;
	else if(name == "LocalB")
		localB << value;
	else if(name == "Min")
		min = value.GetDouble();
	else if(name == "Max")
		max = value.GetDouble();
	else if(name == "Stiffness")
		stiffness = value.GetDouble();
	else if(name == "Damping")
		damping = value.GetDouble();
	Joint::OnPropertyGridChange(name,value);
}
