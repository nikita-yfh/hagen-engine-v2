#include "Chain.hpp"
#include "Body.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

void Chain::Draw(const Colors &colors) const{
	if(!points)
		return;
	parent->Transform();
	ApplyBorder(colors);
	glBegin(GL_LINE_STRIP);
	for(Point *point=points; point; point=point->next)
		glutils::Vertex(*point);
	glEnd();
	if(IsSelected()){
		glBegin(GL_LINES);
		for(const Point *point=points; point && point->next; point = point->next) {
			Point *point2=point->next;
			b2Vec2 middle((*point2+*point)/2.0f);
			b2Vec2 normal((*point2-*point).Skew());
			normal.Normalize();
			normal *= 0.25f;
			b2Vec2 middle2=middle+normal;
			glutils::Vertex(middle);
			glutils::Vertex(middle2);
		}
		glEnd();
	}
}
void Chain::DrawPoints(const Colors &colors) const{
	parent->Transform();
	int index=1;
	for(const Point *point=points; point; point=point->next)
		DrawPoint(colors,index++,*point);
	if(IsSelected()){
		for(const Point *p1=points; p1 && p1->next; p1=p1->next){
			const Point *p2=p1->next;
			b2Vec2 point((*p1+*p2)/2.0f);
			DrawPoint(colors,index++,point);
		}
	}
}

bool Chain::TestPoint(const b2Vec2 &point) const{
	return false; // :)
}
bool Chain::CanBeDynamic()const{
	return false;
}
void Chain::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "chain", allocator);
	Poly::SavePoints(value, allocator);
	Fixture::ToJSON(value, allocator);
}
