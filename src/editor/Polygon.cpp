#include "Polygon.hpp"
#include "Body.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

Polygon::~Polygon(){
	ClearPoints();
}
void Polygon::ClearPoints(){
	while(points){
		Point *next = points->next;
		delete points;
		points = next;
	}
	points = nullptr;
}
void Polygon::AddPoint(b2Vec2 _point){
	Point *point = new Point(_point);
	point->next = points;
	points = point;
}

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
	parent->Transform();
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
	parent->Transform();
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

bool Polygon::UpdatePoints(const Mouse &_mouse){
	const Mouse mouse = parent->GetLocalMouse(_mouse);
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
	return UpdateBody(mouse);
}
bool Polygon::Create(const Mouse &_mouse){
	const Mouse mouse = parent->GetLocalMouse(_mouse);
	int index=1;
	if(!points)
		points=new Point();
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
void Polygon::SavePoints(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	rapidjson::Value array(rapidjson::kArrayType);
	for(Point *point=points;point;point=point->next)
		array.PushBack(jsonutils::Value<b2Vec2>(*point, allocator), allocator);
	value.AddMember("points", array, allocator);
}
void Polygon::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "polygon", allocator);
	SavePoints(value, allocator);
	Fixture::Save(value, allocator);
}
bool Polygon::Load(const rapidjson::Value &value){
	if(!value.HasMember("points"))
		return true;
	const rapidjson::Value &array = value["points"];
	if(!array.IsArray() || array.Size() < 3)
		return true;
	ClearPoints();
	for(int i=0; i<array.Size(); i++)
		AddPoint(jsonutils::Get<b2Vec2>(array[i])); 
	return Fixture::Load(value);
}

	


