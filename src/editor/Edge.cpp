#include "Edge.hpp"
#include "GLUtils.hpp"
#include "Body.hpp"

Edge::Edge()
	:p1(b2Vec2_zero), p2(b2Vec2_zero) {}

void Edge::Draw(const Colors &colors) const{
	parent->Transform();
	ApplyBorder(colors);
	glutils::DrawLine(p1,p2);
}
void Edge::DrawPoints(const Colors &colors) const{
	parent->Transform();
	DrawPoint(colors, 1, p1);
	DrawPoint(colors, 2, p2);
}
bool Edge::UpdatePoints(const Mouse &_mouse){
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	return UpdatePoint(mouse, 1, p1) ||
			UpdatePoint(mouse, 2, p2) ||
			UpdateBody(mouse);
}
bool Edge::Create(const Mouse &_mouse){
	UpdatePoints(_mouse);
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	if(selected==1)
		p2=p1;
	if(mouse.pressed)
		if(selected==1)
			selected=2;
		else if(selected==2)
			return true;
	return false;
}
void Edge::Reverse(){
	b2Vec2 temp = p1;
	p1 = p2;
	p2 = temp;
}

bool Edge::TestPoint(const b2Vec2 &point) const{
	return false; // :)
}
bool Edge::CanBeDynamic() const{
	return false;
}
void Edge::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n){
		pg->GetProperty("point1")->SetValue(WXVARIANT(p1));
		pg->GetProperty("point2")->SetValue(WXVARIANT(p2));
	}else{
		pg->Append(new Vec2Property("point1", wxPG_LABEL, p1));
		pg->Append(new Vec2Property("point2", wxPG_LABEL, p2));
	}
	Fixture::UpdatePropertyGrid(pg,n);
}
void Edge::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "point1")
		p1 << value;
	else if(name == "point2")
		p2 << value;
	Fixture::OnPropertyGridChange(name,value);
}
void Edge::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "edge", allocator);
	value.AddMember("point1", jsonutils::ToJSON(p1, allocator), allocator);
	value.AddMember("point2", jsonutils::ToJSON(p2, allocator), allocator);
	Fixture::ToJSON(value, allocator);
}
bool Edge::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "point1", p1) &&
		jsonutils::GetMember(value, "point2", p2) &&
		Fixture::FromJSON(value);
}

