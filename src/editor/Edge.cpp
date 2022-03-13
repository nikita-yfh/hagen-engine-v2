#include "Edge.hpp"
#include "GLUtils.hpp"
#include "Body.hpp"

Edge::Edge()
	:p1(b2Vec2_zero), p2(b2Vec2_zero) {}

void Edge::Draw(const Colors &colors) const{
	ApplyBorder(colors);
	glutils::DrawLine(p1,p2);
}
void Edge::DrawPoints(const Colors &colors) const{
	DrawPoint(colors, 1, p1);
	DrawPoint(colors, 2, p2);
}
bool Edge::UpdatePoints(const Mouse &mouse){
	return UpdatePoint(mouse, 1, p1) ||
			UpdatePoint(mouse, 2, p2);
}
bool Edge::Create(const Mouse &_mouse){
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	UpdatePoints(mouse);
	if(selected==1)
		p2=p1;
	if(mouse.pressed)
		if(selected==1)
			selected=2;
		else if(selected==2)
			return true;
	return false;
}

bool Edge::TestPoint(const b2Vec2 &point) const{
	return false; // :)
}
bool Edge::CanBeDynamic() const{
	return false;
}
void Edge::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n){
		pg->GetProperty("Point1")->SetValue(WXVARIANT(p1));
		pg->GetProperty("Point2")->SetValue(WXVARIANT(p2));
	}else{
		pg->Append(new Vec2Property("Point1", wxPG_LABEL, p1));
		pg->Append(new Vec2Property("Point2", wxPG_LABEL, p2));
	}
	Fixture::UpdatePropertyGrid(pg,n);
}
void Edge::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "Point1")
		p1 << value;
	else if(name == "Point2")
		p2 << value;
	Fixture::OnPropertyGridChange(name,value);
}
