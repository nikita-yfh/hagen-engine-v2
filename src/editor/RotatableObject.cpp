#include "RotatableObject.hpp"

RotatableObject::RotatableObject() :
	position(b2Vec2_zero),
	angle(0.0f),
	rotate(0.0f) {}

Mouse RotatableObject::GetLocalMouse(const Mouse &mouse) const{
	b2Transform transform(position,rotate);
	return mouse*transform;
}
void RotatableObject::Transform() const{
	glPushMatrix();
	glutils::Translate(position);
	glutils::Rotate(angle);
}
void RotatableObject::SetAngle(float a){
	angle = a;
	rotate.Set(a);
}
const b2Vec2 &RotatableObject::GetPosition() const{
	return position;
}
float RotatableObject::GetAngle() const{
	return angle;
}
void RotatableObject::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if (!n){
		pg->GetProperty("Position")->SetValue(WXVARIANT(position));
		pg->GetProperty("Angle")->SetValue(glutils::RadToDeg(angle));
	}else{
		pg->Append(new Vec2Property("Position", wxPG_LABEL, position));
		pg->Append(new wxFloatProperty("Angle", wxPG_LABEL, glutils::RadToDeg(angle)));
	}
	Object::UpdatePropertyGrid(pg,n);
}
void RotatableObject::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	if(name == "Position")
		position << value;
	else if(name == "Angle")
		SetAngle(glutils::DegToRad(value.GetDouble()));
	Object::OnPropertyGridChange(name,value);
}

