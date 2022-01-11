#include "PointJoint.hpp"
#include <wx/glcanvas.h>

PointJoint::PointJoint()
	:position(b2Vec2_zero){selected=1;}

void PointJoint::DrawJoint(const Colors &colors) const{
	DrawConnections(colors,position);
}
void PointJoint::DrawPoints(const Colors &colors) const{
	DrawPoint(colors,1,position);
}
bool PointJoint::UpdatePoints(const Mouse &mouse){
	return UpdatePoint(mouse,1,position);
}
bool PointJoint::Create(const Mouse &mouse){
	UpdatePoints(mouse);
	return mouse.pressed;
}
void PointJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n)
		pg->GetProperty("Position")->SetValue(WXVARIANT(position));
	else
		pg->Append(new Vec2Property("Position", wxPG_LABEL, position));
	Joint::UpdatePropertyGrid(pg,n);
}
void PointJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "Position")
		position << value;
	OnPropertyGridChange(name,value);
}
