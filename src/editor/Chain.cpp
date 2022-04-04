#include "Chain.hpp"
#include "Body.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

void Chain::Draw(const Colors &colors) const{
	parent->Transform();
	ApplyBorder(colors);
	glBegin(GL_LINE_STRIP);
	for(const Point *point=points->next;point&&point->next;point=point->next)
		glutils::Vertex(*point);
	glEnd();
	ApplyFill(colors);
	glBegin(GL_LINES);
	int index=0;
	int count=GetPointCount();
	for(const Point *point=points;point;point=point->next,index++)
		if(index==0 || index==1 || index==count-1 || index==count-2)
			glutils::Vertex(*point);
	glEnd();
			
	if(IsSelected()){
		glBegin(GL_LINES);
		for(const Point *point=points->next;
				point&&point->next&&point->next->next;point=point->next){
			Point *point2=point->next;
			b2Vec2 middle((*point+*point2)/2.0f);
			b2Vec2 normal((*point-*point2).Skew());
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
	for(const Point *point=points;point;point=point->next)
		DrawPoint(colors,index++,*point);
	if(IsSelected()){
		for(const Point *p1=points->next;p1&&p1->next&&p1->next->next;p1=p1->next){
			const Point *p2=p1->next;
			b2Vec2 point((*p1+*p2)/2.0f);
			DrawPoint(colors,index++,point);
		}
	}
}
bool Chain::UpdatePoints(const Mouse &_mouse){
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	int index=1;
	for(Point *point=points;point;point=point->next)
		if(UpdatePoint(mouse,index++,*point))
			return true;
	for(Point *p1=points->next;p1&&p1->next&&p1->next->next;p1=p1->next){
		Point *p2=p1->next;
		b2Vec2 point((*p1+*p2)/2.0f);
		if(*p1==*p2){ //delete p2
			Point *child=p2->next;
			delete p2;
			p1->next=child;
		}
		if(UpdatePoint(mouse,index++,point)){
			p1->next=new Point(point);
			p1->next->next=p2;
			selected=index-GetPointCount()+2;
			return true;
		}
	}
	return UpdateBody(mouse);
}
bool Chain::TestPoint(const b2Vec2 &point) const{
	return false; // :)
}
bool Chain::CanBeDynamic()const{
	return false;
}
void Chain::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "chain", allocator);
	Poly::SavePoints(value, allocator);
}
