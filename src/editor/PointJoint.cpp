#include "PointJoint.hpp"
#include <wx/glcanvas.h>

PointJoint::PointJoint()
	:position(b2Vec2_zero){selected=1;}

void PointJoint::Draw(const Colors &colors) const{
	ApplyDrawJoint(colors);
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
		pg->GetProperty("position")->SetValue(WXVARIANT(position));
	else
		pg->Append(new Vec2Property("position", wxPG_LABEL, position));
	Joint::UpdatePropertyGrid(pg,n);
}
void PointJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "position")
		position << value;
	OnPropertyGridChange(name,value);
}
void PointJoint::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const {
	value.AddMember("position", jsonutils::ToJSON(position, allocator), allocator);
	Joint::ToJSON(value, allocator);
}
bool PointJoint::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "position", position) &&
		Joint::FromJSON(value);
}
