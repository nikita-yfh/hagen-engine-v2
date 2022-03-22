#include "Image.hpp"
#include "GLUtils.hpp"

Image::Image(const Texture *_texture, const float &_scale) :
	texture(_texture),
	scale(_scale),
	bindBody(nullptr){}

void Image::Transform() const{
	RotatableObject::Transform();
	glutils::Scale(texture->GetWidth()/2.0f/scale, texture->GetHeight()/2.0f/scale);
}

void Image::Draw(const Colors &colors) const{
	Transform();
	texture->Activate();
	glBegin(GL_QUADS);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glTexCoord2i(0,0); glVertex2i(-1, -1);
	glTexCoord2i(0,1); glVertex2i(-1,  1);
	glTexCoord2i(1,1); glVertex2i( 1,  1);
	glTexCoord2i(1,0); glVertex2f( 1, -1);
	glEnd();
	texture->Deactivate();
	TransformBack();
}
void Image::DrawPoints(const Colors &colors) const{
	if(IsSelected()){
		Transform();
		colors.Apply(COLOR_ACTIVE);
		glLineWidth((selected == 2) ? 2.0f : 1.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2i(-1, -1);
		glVertex2i(-1,  1);
		glVertex2i( 1,  1);
		glVertex2f( 1, -1);
		glEnd();
		TransformBack();
	}
}
bool Image::TestPoint(const b2Vec2 &point) const{
	return abs(point.x)*scale*2 < texture->GetWidth() &&
			abs(point.y)*scale*2 < texture->GetHeight();
}
bool Image::UpdatePoints(const Mouse &_mouse){
	const Mouse mouse = GetLocalMouse(_mouse);

	if(selected != 2 && mouse.pressed && TestPoint(mouse.position)){
		delta = mouse.camera.ToGrid(mouse.position);
	 	selected = 2;
	}else if(selected == 2)
		position = mouse.camera.ToGrid(_mouse.position - delta);

	return selected == 2 || UpdatePoint(_mouse, 1, position);
}
bool Image::Create(const Mouse &mouse){
	UpdatePoint(mouse,1,position);
	return true;
}
bool Image::TryRemove(const void *object){
	if(texture == object)
		return false;
	if(bindBody != object)
		return true;
	position += bindBody->GetPosition();
	angle += bindBody->GetAngle();
	bindBody = nullptr;
	return true;
}
void Image::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	RotatableObject::UpdatePropertyGrid(pg,n);
}
void Image::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	RotatableObject::OnPropertyGridChange(name, value);
}

