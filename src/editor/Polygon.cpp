#include "Polygon.hpp"
#include "Body.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

Polygon::Polygon()
	: points(new Point()) {selected=1;}

int Polygon::GetPointCount() const{
	int count=0;
	for(Point *point=points;point;point=point->next)
		count++;
	return count;
}
bool Polygon::CancelCreating(){
	if(points != nullptr) { //delete last point
        if(points->next == nullptr){
			delete points;
			points = nullptr;
		} else {
			Point* temp = points;
			while(temp->next->next != nullptr)
				temp = temp->next;
			delete temp->next;
			temp->next=nullptr;
		}
	}
	if(GetPointCount()<3)
		return false;
	return true;
}
void Polygon::Draw(const Colors &colors) const{
	ApplyFill(colors);
	glBegin(GL_POLYGON);
	for(Point *point=points;point;point=point->next)
		glutils::Vertex(*point);
	glEnd();
	ApplyBorder(colors);
	glBegin(GL_LINE_LOOP);
	for(Point *point=points;point;point=point->next)
		glutils::Vertex(*point);
	glEnd();
}
void Polygon::DrawPoints(const Colors &colors) const{
	int index=1;
	for(const Point *point=points;point;point=point->next)
		DrawPoint(colors,index++,*point);
	if(IsSelected()){
		for(const Point *p1=points;p1;p1=p1->next){
			const Point *p2=p1->next;
			if(p2==nullptr)p2=points;
			b2Vec2 point((*p1+*p2)/2.0f);
			DrawPoint(colors,index++,point);
		}
	}
}

bool Polygon::UpdatePoints(const Mouse &mouse){
	int index=1;
	for(Point *point=points;point;point=point->next)
		if(UpdatePoint(mouse,index++,*point))
			return true;
	for(Point *p1=points;p1;p1=p1->next){
		Point *p2=p1->next;
		if(p2==nullptr)p2=points;
		b2Vec2 point((*p1+*p2)/2.0f);
		if(*p1==*p2){ //delete p2
			Point *child=p2->next;
			delete p2;
			p1->next=child;
		}
		if(UpdatePoint(mouse,index++,point)){
			p1->next=new Point(point);
			p1->next->next=p2;
			selected=index-GetPointCount()+1;
			return true;
		}
	}
	return false;
}
bool Polygon::CreateFixture(const Mouse &mouse){
	int index=1;
	for(Point *point=points;point;point=point->next)
		if(!point->next){
			UpdatePoint(mouse,index++,*point);
			if(mouse.pressed){
				point->next=new Point(*point);
				return false;
			}
		}

	return false;
}
bool Polygon::TestPoint(const b2Vec2 &point) const{
	bool result=false;
	for(const Point *a=points;a;a=a->next){
		const Point *b=a->next;
		if(b==nullptr)b=points;
		
		if(std::min(a->y,b->y)<point.y && std::max(a->y,b->y)>point.y &&
				a->x+(point.y-a->y)/(b->y-a->y)*(b->x-a->x)<point.x)
			result=!result;
	}
	return result;
}
