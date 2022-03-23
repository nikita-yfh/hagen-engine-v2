#include "Image.hpp"
#include "GLUtils.hpp"

bool Image::enabled = false;

Image::Image(const Texture *_texture, const Body *_bindBody, const float &_scale) :
	texture(_texture), scale(_scale), bindBody(_bindBody), layer(0) {}

void Image::Transform() const{
	if(bindBody)
		bindBody->Transform();
	Rotatable::Transform();
	glutils::Scale(texture->GetWidth()/2.0f/scale, texture->GetHeight()/2.0f/scale);
}
Mouse Image::GetBodyMouse(const Mouse &mouse) const{
	if(bindBody)
		return bindBody->GetLocalMouse(mouse);
	return mouse;
}
void Image::Draw(const Colors &colors) const{
	if(!enabled)
		return;
	Transform();
	texture->Activate();
	glBegin(GL_QUADS);
	glColor4f(1.0f,1.0f,1.0f,0.5f);
	glTexCoord2i(0,0); glVertex2i(-1, -1);
	glTexCoord2i(0,1); glVertex2i(-1,  1);
	glTexCoord2i(1,1); glVertex2i( 1,  1);
	glTexCoord2i(1,0); glVertex2f( 1, -1);
	glEnd();
	texture->Deactivate();
}
void Image::DrawPoints(const Colors &colors) const{
	if(!enabled)
		return;
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
	}
}
bool Image::TestPoint(const b2Vec2 &point) const{
	return abs(point.x)*scale*2 < texture->GetWidth() &&
			abs(point.y)*scale*2 < texture->GetHeight();
}
bool Image::UpdatePoints(const Mouse &_mouse){
	if(!enabled)
		return false;
	const Mouse mouse = GetBodyMouse(_mouse);
	const Mouse localMouse = GetLocalMouse(mouse);

	if(selected != 2 && mouse.pressed && TestPoint(localMouse.position)){
		delta = mouse.camera.ToGrid(localMouse.position);
	 	selected = 2;
	}else if(selected == 2)
		position = mouse.camera.ToGrid(mouse.position - delta);

	return selected == 2 || UpdatePoint(mouse, 1, position);
}
bool Image::Create(const Mouse &mouse){
	UpdatePoint(GetBodyMouse(mouse),1,position);
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
int8_t Image::GetLayer() const {
	return layer;
}
void Image::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	Rotatable::UpdatePropertyGrid(pg,n);
	if(n){
		pg->Append(new wxIntProperty("Layer", wxPG_LABEL, layer));
	}
	Object::UpdatePropertyGrid(pg,n);
}
void Image::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	Rotatable::OnPropertyGridChange(name, value);
	if (name == "Layer")
		layer = value.GetLong();
	Object::OnPropertyGridChange(name, value);
}

