#include "Body.hpp"
#include "GLUtils.hpp"
#include "EditorMain.hpp"
#include <wx/glcanvas.h>

Body::Body(b2BodyType _type) :
		position(b2Vec2_zero),
		type(_type),
		angle(0.0f),
		linearVelocity(0.0f,0.0f),
		angularVelocity(0.0f),
		linearDamping(0.0f),
		angularDamping(0.0f),
		allowSleep(true),
		awake(true),
		fixedRotation(false),
		bullet(false),
		enabled(true),
		gravityScale(1.0f) {}

void Body::Draw(const Colors &colors) const{
}
void Body::DrawPoints(const Colors &colors) const{
	DrawPoint(colors, 1, position);
}
bool Body::UpdatePoints(const Mouse &mouse){
	return UpdatePoint(mouse,1,position);
}
bool Body::Create(const Mouse &mouse){
	UpdatePoint(mouse,1,position);
	return mouse.pressed;
}
Mouse Body::GetLocalMouse(const Mouse &mouse) const{
	b2Transform transform(position,b2Rot(angle));
	return mouse*transform;
}
void Body::Transform() const{
	glPushMatrix();
	glutils::Translate(position);
	glutils::Rotate(angle);
}
void Body::TransformBack() const{
	glPopMatrix();
}
Color Body::GetColor() const{
	switch(type){
	case b2_dynamicBody:
		return COLOR_BODY_DYNAMIC;
	case b2_staticBody:
		return COLOR_BODY_STATIC;
	case b2_kinematicBody:
		return COLOR_BODY_KINEMATIC;
	}
	return COLOR_DARK;
}
float Body::GetLineWidth() const{
	if(IsSelected())
		return 2.0f;
	return 1.0f;
}
const b2Vec2 &Body::GetPosition() const{
	return position;
}
float Body::GetAngle() const{
	return angle;
}
void Body::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if (!n)
		pg->GetProperty("Position")->SetValue(WXVARIANT(position));
	else{
 		wxPGChoices eech;
 		eech.Add("static");
 		eech.Add("kinematic");
 		eech.Add("dynamic");
		pg->Append(new wxEnumProperty("Type", wxPG_LABEL, eech, (int)type));
		pg->Append(new Vec2Property("Position", wxPG_LABEL, position));
		pg->Append(new wxFloatProperty("Angle", wxPG_LABEL, glutils::RadToDeg(angle)));
		pg->Append(new Vec2Property("LinearVelocity", wxPG_LABEL, linearVelocity));
		pg->Append(new wxFloatProperty("AngularVelocity", wxPG_LABEL, angularVelocity));
		pg->Append(new wxFloatProperty("LinearDamping", wxPG_LABEL, linearDamping));
		pg->Append(new wxFloatProperty("AngularDamping", wxPG_LABEL, angularDamping));
		pg->Append(new wxBoolProperty("AllowSleep", wxPG_LABEL, allowSleep));
		pg->Append(new wxBoolProperty("Awake", wxPG_LABEL, awake));
		pg->Append(new wxBoolProperty("FixedRotation", wxPG_LABEL, fixedRotation));
		pg->Append(new wxBoolProperty("Bullet", wxPG_LABEL, bullet));
		pg->Append(new wxBoolProperty("Enabled", wxPG_LABEL, enabled));
		pg->Append(new wxFloatProperty("GravityScale", wxPG_LABEL, gravityScale));
	}
	Object::UpdatePropertyGrid(pg,n);
}
void Body::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	if (name == "Type")
		type = (b2BodyType)value.GetLong();
	else if(name == "Position")
		position << value;
	else if(name == "Angle")
		angle = glutils::DegToRad(value.GetDouble());
	else if(name == "LinearVelocity")
		linearVelocity << value;
	else if(name == "AngularVelocity")
		angularVelocity = value.GetDouble();
	else if (name == "LinearDamping")
		linearDamping = value.GetDouble();
	else if (name == "AngularDamping")
		angularDamping = value.GetDouble();
	else if (name == "AllowSleep")
		allowSleep = value.GetBool();
	else if (name == "Awake")
		awake = value.GetBool();
	else if (name == "FixedRotation")
		fixedRotation = value.GetBool();
	else if (name == "Bullet")
		bullet = value.GetBool();
	else if (name == "Enabled")
		enabled = value.GetBool();
	else if (name == "GravityScale")
		gravityScale = value.GetDouble();
	Object::OnPropertyGridChange(name,value);
}

