#include "Circle.hpp"
#include "GLUtils.hpp"
#include "Body.hpp"

Circle::Circle()
		:position(b2Vec2_zero), radius(0.0f) {
	selected=2;
}

void Circle::Draw(const Colors &colors) const{
	parent->Transform();
	const Color color=parent->GetColor();
	ApplyFill(colors);
	glutils::DrawCircle(position, radius,true);
	ApplyBorder(colors);
	glutils::DrawCircle(position, radius,false);
	glutils::DrawLine(position, position + b2Vec2(radius, 0.0f));
}
void Circle::DrawPoints(const Colors &colors) const{
	parent->Transform();
	DrawPoint(colors, 1, position + b2Vec2(radius,0.0f));
	DrawPoint(colors, 2, position);
}
bool Circle::UpdatePoints(const Mouse &_mouse){
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	b2Vec2 point(position + b2Vec2(radius,0.0f));
	if(UpdatePoint(mouse, 1, point)){
		radius=mouse.camera.ToGrid(b2Distance(point, position));
		return true;
	}
	return UpdatePoint(mouse, 2, position) || UpdateBody(mouse);
}
bool Circle::Create(const Mouse &_mouse){
	UpdatePoints(_mouse);
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	if(mouse.pressed)
		if(selected==2)
			selected=1;
		else if(selected==1)
			return true;
	return false;
}

bool Circle::TestPoint(const b2Vec2 &point) const{
	return b2DistanceSquared(point,position)<=radius*radius;
}
void Circle::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(!n){
		pg->GetProperty("position")->SetValue(WXVARIANT(position));
		pg->GetProperty("radius")->SetValue(radius);
	}else{
		pg->Append(new Vec2Property("position", wxPG_LABEL, position));
		pg->Append(new wxFloatProperty("radius", wxPG_LABEL, radius));
	}
	Fixture::UpdatePropertyGrid(pg,n);
}
void Circle::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "position")
		position << value;
	else if(name == "radius")
		radius = value.GetDouble();
	Fixture::OnPropertyGridChange(name,value);
}
void Circle::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "circle", allocator);
	value.AddMember("position", jsonutils::Value(position, allocator), allocator);
	value.AddMember("radius", radius, allocator);
	Fixture::Save(value, allocator);
}
bool Circle::Load(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "position", position) ||
		jsonutils::GetMember(value, "radius", radius) ||
		Fixture::Load(value);
}

