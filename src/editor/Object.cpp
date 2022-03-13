#include "Object.hpp"
#include <wx/glcanvas.h>

const float Object::activeSize=6.0f;
const float Object::inactiveSize=4.0f;
const float Object::borderAlpha=1.0f;
const float Object::fillAlpha=0.4f;

Object::Object() : selected(1), next(nullptr) {}

void Object::DrawPoint(const Colors &colors, int index, const b2Vec2 &pos) const{
	if(index==selected || selected==-1)
		colors.Apply(COLOR_ACTIVE);
	else
		colors.Apply(COLOR_INACTIVE);

	if(index==selected)
		glPointSize(activeSize*2);
	else
		glPointSize(inactiveSize*2);

	glBegin(GL_POINTS);
	glVertex2f(pos.x,pos.y);
	glEnd();
}
bool Object::UpdatePoint(const Mouse &mouse, int index, b2Vec2 &pos, bool grid){
	if(mouse.pressed){
		float zoomedSize=inactiveSize/mouse.camera.zoom;
		if(!IsSelectedPoint() && b2DistanceSquared(mouse.position,pos)<zoomedSize*zoomedSize)
			selected=index;
	}else if(selected==index){
		if(grid)
			pos=mouse.camera.ToGrid(mouse.position);
		else
			pos=mouse.position;
	}
	return selected==index;
}
bool Object::CancelCreating(){
	return true;
}
bool Object::Create(const Mouse &mouse){
	return true;
}
bool Object::TryRemove(const void *object){
	return true;
}
void Object::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n)
		pg->Append(new wxStringProperty("ID", wxPG_LABEL, id));
}
void Object::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "ID")
		id = value.GetString();
}

