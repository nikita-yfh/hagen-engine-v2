#include "Joint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

Joint::Joint()
	:a(nullptr),b(nullptr),collideConnected(false), next(nullptr) {}
void Joint::Draw(const Colors &colors) const{
	bool selected=IsSelected();
	if(a)selected |= a->IsSelected();
	if(b)selected |= b->IsSelected();
	glLineWidth(selected?2.0f:1.0f);
	colors.Apply(COLOR_JOINT,borderAlpha);
	DrawJoint(colors);
}
void Joint::SetBodies(Body *_a, Body *_b){
	a=_a;
	b=_b;
}
bool Joint::HasBody(const Body *body) const{
	return a==body || b==body;
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
		pg->Append(new wxBoolProperty("CollideConnected", wxPG_LABEL, collideConnected));
	Object::UpdatePropertyGrid(pg,n);
}
void Joint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "CollideConnected")
		collideConnected = value.GetBool();
	Object::OnPropertyGridChange(name, value);
}