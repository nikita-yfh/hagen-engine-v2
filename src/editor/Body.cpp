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
		gravityScale(1.0f),
		fixtures(nullptr),
		next(nullptr) {
	selected=1;
}
Body::~Body(){
	ClearFixtures();
}

void Body::ClearFixtures(){
	while(fixtures){
		Fixture *child=fixtures->next;
		delete fixtures;
		fixtures=child;
	}
}
void Body::AddFixture(Fixture *fixture){
	fixture->next=fixtures;
	fixtures=fixture;
	fixture->parent=this;
}
void Body::DeleteSelected(){
	Fixture *prev=nullptr;
	for(Fixture *fixture=fixtures; fixture;){
		if(fixture->IsSelected()){
			if(prev)
				prev->next=fixture->next;
			else
				fixtures=fixture->next;
			Fixture *child=fixture->next;
			delete fixture;
			fixture=child;
		}else{
			prev=fixture;
			fixture=fixture->next;
		}
	}
}
void Body::Draw(const Colors &colors) const{
	Transform();
	glLineWidth(GetLineWidth());
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		fixture->Draw(colors);
	TransformBack();
}
void Body::DrawPoints(const Colors &colors) const{
	DrawPoint(colors, 1, position);
	Transform();
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		fixture->DrawPoints(colors);
	TransformBack();
}
bool Body::UpdatePoints(const Mouse &mouse){
	Mouse localMouse=GetLocalMouse(mouse);
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		if(fixture->UpdatePoints(localMouse))
			return true;
	static b2Vec2 delta;
	if(mouse.pressed){
	  	if(TestPoint(localMouse.position)){
			selected=2;
			delta=mouse.camera.ToGrid(localMouse.position);
		}
	}else if(selected==2)
		position=mouse.camera.ToGrid(mouse.position-delta);

	return selected==2 || UpdatePoint(mouse,1,position);
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
bool Body::TestPoint(const b2Vec2 &point) const{
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		if(fixture->TestPoint(point))
			return true;
	return false;
}
void Body::UnselectPoints(){
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		fixture->UnselectPoints();
	if(IsSelectedPoint())
		Select();
}
void Body::SelectAll(){
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		fixture->Select();
	Select();
}
void Body::UnselectAll(){
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		fixture->Unselect();
	Unselect();
}
int Body::GetSelectedCount() const{
	int count;
	if(IsSelected())
		count++;
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		if(fixture->IsSelected())
			count++;
	return count;
}
Object *Body::GetFirstSelected(){
	if(IsSelected())
		return this;
	for(Fixture *fixture=fixtures;fixture;fixture=fixture->next)
		if(fixture->IsSelected())
			return fixture;
	return nullptr;
}
void Body::SetType(b2BodyType _type){
	type=_type;
	Fixture *prev=nullptr;
	for(Fixture *fixture=fixtures; fixture;){
		if(!fixture->CanBeDynamic()){
			if(prev)
				prev->next=fixture->next;
			else
				fixtures=fixture->next;
			Fixture *child=fixture->next;
			delete fixture;
			fixture=child;
		}else{
			prev=fixture;
			fixture=fixture->next;
		}
	}
}
float Body::GetLineWidth() const{
	if(IsSelected())
		return 2.0f;
	return 1.0f;
}
const b2Vec2 &Body::GetPosition() const{
	return position;
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

