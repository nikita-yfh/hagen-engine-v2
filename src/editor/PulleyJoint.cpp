#include "PulleyJoint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

PulleyJoint::PulleyJoint()
	:groundA(b2Vec2_zero), groundB(b2Vec2_zero),
	localA(b2Vec2_zero), localB(b2Vec2_zero), ratio(1.0f) {selected=1;}

void PulleyJoint::DrawJoint(const Colors &colors) const{
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
		pg->GetProperty("GroundA")->SetValue(WXVARIANT(groundA));
		pg->GetProperty("GroundB")->SetValue(WXVARIANT(groundB));
		pg->GetProperty("LocalA")->SetValue(WXVARIANT(localA));
		pg->GetProperty("LocalB")->SetValue(WXVARIANT(localB));
	}else{
		pg->Append(new Vec2Property("GroundA", wxPG_LABEL, groundA));
		pg->Append(new Vec2Property("GroundB", wxPG_LABEL, groundB));
		pg->Append(new Vec2Property("LocalB", wxPG_LABEL, localA));
		pg->Append(new Vec2Property("LocalA", wxPG_LABEL, localB));
		pg->Append(new wxFloatProperty("Ratio", wxPG_LABEL, ratio));
	}
	Joint::UpdatePropertyGrid(pg,n);
}
void PulleyJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "GroundA")
		groundA << value;
	else if(name == "GroundB")
		groundB << value;
	else if(name == "LocalA")
		localA << value;
	else if(name == "LocalB")
		localB << value;
	else if(name == "Ratio")
		ratio = value.GetDouble();
	Joint::OnPropertyGridChange(name,value);
}


