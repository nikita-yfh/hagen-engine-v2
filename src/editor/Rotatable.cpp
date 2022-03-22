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
		pg->GetProperty("Position")->SetValue(WXVARIANT(position));
		pg->GetProperty("Angle")->SetValue(glutils::RadToDeg(angle));
	}else{
		pg->Append(new Vec2Property("Position", wxPG_LABEL, position));
		pg->Append(new wxFloatProperty("Angle", wxPG_LABEL, glutils::RadToDeg(angle)));
	}
}
void Rotatable::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	if(name == "Position")
		position << value;
	else if(name == "Angle")
		SetAngle(glutils::DegToRad(value.GetDouble()));
}

