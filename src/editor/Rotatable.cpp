#include "Rotatable.hpp"
#include "GLUtils.hpp"

Rotatable::Rotatable() :
	position(b2Vec2_zero),
	angle(0.0f),
	rotate(0.0f) {}

Mouse Rotatable::GetLocalMouse(const Mouse &mouse) const{
	b2Transform transform(position,rotate);
	return mouse*transform;
}
void Rotatable::Transform() const{
	glutils::Translate(position);
	glutils::Rotate(angle);
}
void Rotatable::SetAngle(float a){
	angle = a;
	rotate.Set(a);
}
const b2Vec2 &Rotatable::GetPosition() const{
	return position;
}
float Rotatable::GetAngle() const{
	return angle;
}
void Rotatable::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if (!n){
		pg->GetProperty("position")->SetValue(WXVARIANT(position));
		pg->GetProperty("angle")->SetValue(glutils::RadToDeg(angle));
	}else{
		pg->Append(new Vec2Property("position", wxPG_LABEL, position));
		pg->Append(new wxFloatProperty("angle", wxPG_LABEL, glutils::RadToDeg(angle)));
	}
}
void Rotatable::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	if(name == "position")
		position << value;
	else if(name == "angle")
		SetAngle(glutils::DegToRad(value.GetDouble()));
}
void Rotatable::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("position", jsonutils::Value(position, allocator), allocator);
	value.AddMember("angle", angle, allocator);
}
bool Rotatable::Load(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "position", position) ||
		jsonutils::GetMember(value, "angle", angle);
}


