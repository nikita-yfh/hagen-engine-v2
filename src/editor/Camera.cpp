#include "Camera.hpp"
#include "Object.hpp"
#include <wx/glcanvas.h>

Camera::Camera()
	:position(0.0f,0.0f), zoom(100.0f), gridSize(0.25f), currentGridSize(0.25f),
		currentAngleGrid(M_PI/12.0f) {}

void Camera::DrawGrid(const Colors &colors, const wxSize &halfsize) const{
	if(currentGridSize*zoom>10){
		colors.Apply(COLOR_GRID);
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		const float &size=currentGridSize;
		for(float x=-fmod(position.x,size)*zoom;x<halfsize.x;x+=size*zoom){
			glVertex2f(x,-halfsize.y);
			glVertex2f(x,halfsize.y);
		}
		for(float x=-fmod(position.x,size)*zoom;x>-halfsize.x;x-=size*zoom){
			glVertex2f(x,-halfsize.y);
			glVertex2f(x,halfsize.y);
		}
		for(float y=-fmod(position.y,size)*zoom;y<halfsize.y;y+=size*zoom){
			glVertex2f(-halfsize.x,y);
			glVertex2f(halfsize.x,y);
		}
		for(float y=-fmod(position.y,size)*zoom;y>-halfsize.y;y-=size*zoom){
			glVertex2f(-halfsize.x,y);
			glVertex2f(halfsize.x,y);
		}
		glEnd();
	}
}
void Camera::Apply() const {
	glScalef(zoom,zoom,1.0f);
	glTranslatef(-position.x,-position.y,0.0f);
}

void Camera::SetGrid(float grid){
	gridSize = grid;
	UpdateGrid(false, false);
}
bool Camera::UpdateGrid(bool shift, bool ctrl){
	float grid=currentGridSize;
	if(ctrl){
		currentGridSize=0.001;
		currentAngleGrid=M_PI/180.0f; //1 degrees
	}else if(shift){
		currentGridSize=gridSize/2.0f;
		currentAngleGrid=M_PI/24.0f; //7.5 degrees
	}else{
		currentGridSize=gridSize;
		currentAngleGrid=M_PI/12.0f; //15 degrees
	}
	return grid!=currentGridSize;
}

b2Vec2 Camera::ConvertToPixels(const b2Vec2 &p) const{
	return zoom*(p-position);
}
b2Vec2 Camera::ConvertFromPixels(const b2Vec2 &p) const{
	return position+p/zoom;
}
b2Vec2 Camera::ToGrid(const b2Vec2 &p) const{
	return b2Vec2(round(p.x/currentGridSize)*currentGridSize,
				round(p.y/currentGridSize)*currentGridSize);
}
float Camera::ToGrid(float p) const{
	return round(p/currentGridSize)*currentGridSize;
}
float Camera::AngleToGrid(float p) const{
	return round(p/currentAngleGrid)*currentAngleGrid;
}
void Camera::Drag(const b2Vec2 &mouse, const b2Vec2 &pos){
	position=mouse-pos/zoom;
}
void Camera::Zoom(float value, const b2Vec2 &mouse, const b2Vec2 &pos){
	zoom*=value;
	Drag(mouse,pos);
}
