#include "Body.hpp"
#include "GLUtils.hpp"
#include "EditorMain.hpp"

Body::Body(b2BodyType _type) :
		type(_type),
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
	glLineWidth(IsSelected()?2.0f:1.0f);
}

void Body::DrawPoints(const Colors &colors) const{
	DrawPoint(colors, 1, position);
}
bool Body::UpdatePoints(const Mouse &mouse){
	if(selected == 2)
		position = mouse.camera.ToGrid(mouse.position-delta);
	return selected == 2 || UpdatePoint(mouse,1,position);
}
bool Body::Create(const Mouse &mouse){
	UpdatePoint(mouse,1,position);
	return mouse.pressed;
}
void Body::BeginDrag(const Mouse &mouse) {
	selected = 2;
	delta = mouse.camera.ToGrid(b2Mul(rotate,mouse.position));
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
void Body::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n){
 		wxPGChoices eech;
 		eech.Add("static");
 		eech.Add("kinematic");
 		eech.Add("dynamic");
		pg->Append(new wxEnumProperty("Type", wxPG_LABEL, eech, (int)type));
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
	RotatableObject::UpdatePropertyGrid(pg,n);
}
void Body::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	if (name == "Type")
		type = (b2BodyType)value.GetLong();
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
	RotatableObject::OnPropertyGridChange(name,value);
}

