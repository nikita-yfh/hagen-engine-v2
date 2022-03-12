#include "Image.hpp"

Image::Image(const Texture *_texture, const float &_scale)
		:position(b2Vec2_zero),
		texture(_texture),
		scale(_scale),
		bindBody(nullptr),
		next(nullptr) {}

void Image::Draw(const Colors &colors) const{

}
void Image::DrawPoints(const Colors &colors) const{
	DrawPoint(colors, 1, position);
}
bool Image::TestPoint(const b2Vec2 &point) const{
	return abs(point.x)*scale < texture->GetWidth() &&
			abs(point.y)*scale < texture->GetHeight();
}
bool Image::UpdatePoints(const Mouse &mouse){
	b2Transform transform(position,b2Rot(angle));
	const Mouse localMouse = mouse*transform;

	/* if(mouse.pressed && TestPoint(localMouse.position)) */
	/* 	selected = 2; */

	return  UpdatePoint(mouse, 1, position);
}
bool Image::Create(const Mouse &mouse){
	UpdatePoint(mouse,1,position);
	return mouse.pressed;
}
