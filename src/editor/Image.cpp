#include "Image.hpp"
#include "GLUtils.hpp"

bool Image::enabled = false;
const float *Image::scale;

Image::Image(const Texture *_texture, const Body *_bindBody) :
	texture(_texture), bindBody(_bindBody), layer(0), repeat(1.0f, 1.0f) {}

void Image::Transform() const{
	if(bindBody)
		bindBody->Transform();
	Rotatable::Transform();
	glutils::Scale(texture->GetWidth()*repeat.x/2.0f/(*scale),
					texture->GetHeight()*repeat.y/2.0f/(*scale));
}
Mouse Image::GetBodyMouse(const Mouse &mouse) const{
	if(bindBody)
		return bindBody->GetLocalMouse(mouse);
	return mouse;
}
void Image::Draw(const Colors&) const{
	Draw(0.5);
}
void Image::DrawPreview() const{
	Draw(1.0f);
}
void Image::Draw(float alpha) const{
	Transform();
	texture->Activate();
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glTexCoord2f(0.0f, 0.0f);			glVertex2i(-1, -1);
	glTexCoord2f(0.0f, repeat.y);		glVertex2i(-1,  1);
	glTexCoord2f(repeat.x, repeat.y);	glVertex2i( 1,  1);
	glTexCoord2f(repeat.x, 0.0f);		glVertex2f( 1, -1);
	glEnd();
	texture->Deactivate();
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
	}
}
bool Image::TestPoint(const b2Vec2 &point) const{
	return abs(point.x)*(*scale)*2.0f/repeat.x < texture->GetWidth() &&
			abs(point.y)*(*scale)*2.0f/repeat.y < texture->GetHeight();
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
	UpdatePoint(GetBodyMouse(mouse), 1,position);
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
	Rotatable::UpdatePropertyGrid(pg, n);
	if(n){
		pg->Append(new wxIntProperty("layer", wxPG_LABEL, layer));
		pg->Append(new Vec2Property("repeat", wxPG_LABEL, repeat));
	}
	Object::UpdatePropertyGrid(pg, n);
}
void Image::OnPropertyGridChange(const wxString& name, const wxVariant& value){
	Rotatable::OnPropertyGridChange(name, value);
	if (name == "layer")
		layer = value.GetLong();
	else if(name == "repeat")
		repeat << value;
	Object::OnPropertyGridChange(name, value);
}
void Image::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const {
	value.AddMember("layer", layer, allocator);
	value.AddMember("repeat", jsonutils::ToJSON(repeat, allocator), allocator);
	value.AddMember("texture", jsonutils::ToJSON(texture->GetName()), allocator);
	if(bindBody)
		value.AddMember("bindBody", jsonutils::ToJSON(bindBody->GetID()), allocator);
	Rotatable::ToJSON(value, allocator);
	Object::ToJSON(value, allocator);
}
bool Image::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "layer", layer) &&
		jsonutils::GetMember(value, "repeat", repeat) &&
		Rotatable::FromJSON(value) &&
		Object::FromJSON(value);
}


