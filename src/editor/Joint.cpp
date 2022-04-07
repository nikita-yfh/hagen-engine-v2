#include "Joint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

Joint::Joint()
	:a(nullptr),b(nullptr),collideConnected(false) {}

void Joint::SetBodies(Body *_a, Body *_b){
	a=_a;
	b=_b;
}
void Joint::ApplyDrawJoint(const Colors &colors) const{
	bool selected=IsSelected();
	if(a)selected |= a->IsSelected();
	if(b)selected |= b->IsSelected();
	glLineWidth(selected?2.0f:1.0f);
	colors.Apply(COLOR_JOINT,borderAlpha);
}
bool Joint::TryRemove(const void *object){
	if(a!=object && b!=object)
		return true;
	return false;
}
void Joint::DrawConnections(const Colors &colors, const b2Vec2 &p1, const b2Vec2 &p2) const{
	const b2Vec2 &posA=a->GetPosition();
	const b2Vec2 &posB=b->GetPosition();
	
	colors.Apply(COLOR_JOINT_CONNECT);
	glBegin(GL_LINES);
	glutils::Vertex(posA);
	glutils::Vertex(p1+posA);
	glutils::Vertex(p2+posB);
	glutils::Vertex(posB);
	glEnd();
}
void Joint::DrawConnections(const Colors &colors, const b2Vec2 &point) const{
	const b2Vec2 &posA=a->GetPosition();
	const b2Vec2 &posB=b->GetPosition();
	
	colors.Apply(COLOR_JOINT_CONNECT);
	glBegin(GL_LINE_STRIP);
	glutils::Vertex(posA);
	glutils::Vertex(point);
	glutils::Vertex(posB);
	glEnd();
}
Mouse Joint::GetLocalMouseA(const Mouse &mouse){
	return mouse+a->GetPosition();
}
Mouse Joint::GetLocalMouseB(const Mouse &mouse){
	return mouse+b->GetPosition();
}
void Joint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n)
		pg->Append(new wxBoolProperty("collideConnected", wxPG_LABEL, collideConnected));
	Object::UpdatePropertyGrid(pg,n);
}
void Joint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "collideConnected")
		collideConnected = value.GetBool();
	Object::OnPropertyGridChange(name, value);
}
void Joint::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("bodyA", jsonutils::Value(a->GetID()), allocator);
	value.AddMember("bodyB", jsonutils::Value(b->GetID()), allocator);
	value.AddMember("collideConnected", collideConnected, allocator);
	Object::ToJSON(value, allocator);
}
bool Joint::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "collideConnected", collideConnected) &&
		Object::FromJSON(value);
}
