#include "Body.hpp"
#include "GLUtils.hpp"
#include "EditorMain.hpp"

static const char *bodyTypes[] = {
	"static",
	"kinematic",
	"dynamic"
};

Body::Body(b2BodyType _type) :
	type(_type),
	linearVelocity(b2Vec2_zero),
	angularVelocity(0.0f),
	linearDamping(0.0f),
	angularDamping(0.0f),
	allowSleep(true),
	awake(true),
	fixedRotation(false),
	bullet(false),
	enabled(true),
	gravityScale(1.0f) {}
Body::Body() : Body(b2_staticBody) {}

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
	Rotatable::UpdatePropertyGrid(pg,n);
	if(n){
 		wxPGChoices eech;
 		eech.Add("static");
 		eech.Add("kinematic");
 		eech.Add("dynamic");
		pg->Append(new wxEnumProperty("type", wxPG_LABEL, eech, (int)type));
		pg->Append(new Vec2Property("linearVelocity", wxPG_LABEL, linearVelocity));
		pg->Append(new wxFloatProperty("angularVelocity", wxPG_LABEL, angularVelocity));
		pg->Append(new wxFloatProperty("linearDamping", wxPG_LABEL, linearDamping));
		pg->Append(new wxFloatProperty("angularDamping", wxPG_LABEL, angularDamping));
		pg->Append(new wxBoolProperty("allowSleep", wxPG_LABEL, allowSleep));
		pg->Append(new wxBoolProperty("awake", wxPG_LABEL, awake));
		pg->Append(new wxBoolProperty("fixedRotation", wxPG_LABEL, fixedRotation));
		pg->Append(new wxBoolProperty("bullet", wxPG_LABEL, bullet));
		pg->Append(new wxBoolProperty("enabled", wxPG_LABEL, enabled));
		pg->Append(new wxFloatProperty("gravityScale", wxPG_LABEL, gravityScale));
	}
	Object::UpdatePropertyGrid(pg,n);
}
void Body::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	Rotatable::OnPropertyGridChange(name,value);
	if (name == "type")
		type = (b2BodyType)value.GetLong();
	else if(name == "linearVelocity")
		linearVelocity << value;
	else if(name == "angularVelocity")
		angularVelocity = value.GetDouble();
	else if (name == "linearDamping")
		linearDamping = value.GetDouble();
	else if (name == "angularDamping")
		angularDamping = value.GetDouble();
	else if (name == "allowSleep")
		allowSleep = value.GetBool();
	else if (name == "awake")
		awake = value.GetBool();
	else if (name == "fixedRotation")
		fixedRotation = value.GetBool();
	else if (name == "bullet")
		bullet = value.GetBool();
	else if (name == "enabled")
		enabled = value.GetBool();
	else if (name == "gravityScale")
		gravityScale = value.GetDouble();
	Object::OnPropertyGridChange(name,value);
}

void Body::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", jsonutils::String(bodyTypes[type]), allocator);
	value.AddMember("linearVelocity", jsonutils::Value(linearVelocity, allocator), allocator);
	value.AddMember("linearDamping", linearDamping, allocator);
	value.AddMember("angularVelocity", angularVelocity, allocator);
	value.AddMember("allowSleep", allowSleep, allocator);
	value.AddMember("awake", awake, allocator);
	value.AddMember("fixedRotation", fixedRotation, allocator);
	value.AddMember("bullet", bullet, allocator);
	value.AddMember("enabled", enabled, allocator);
	value.AddMember("gravityScale", gravityScale, allocator);
	Rotatable::ToJSON(value, allocator);
	Object::ToJSON(value, allocator);
}
bool Body::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "type", bodyTypes, 3, type) &&
		jsonutils::GetMember(value, "linearVelocity", linearVelocity) &&
		jsonutils::GetMember(value, "angularVelocity", angularVelocity) &&
		jsonutils::GetMember(value, "linearDamping", linearDamping) &&
		jsonutils::GetMember(value, "allowSleep", allowSleep) &&
		jsonutils::GetMember(value, "awake", awake) &&
		jsonutils::GetMember(value, "fixedRotation", fixedRotation) &&
		jsonutils::GetMember(value, "bullet", bullet) &&
		jsonutils::GetMember(value, "enabled", enabled) &&
		jsonutils::GetMember(value, "gravityScale", gravityScale) &&
		Rotatable::FromJSON(value) &&
		Object::FromJSON(value);
}

