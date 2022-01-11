#include "Color.hpp"

#include "glad/glad.h"

using namespace ImGui;

Color::Color(const ImVec4 &vec){
	r=vec.x;
	g=vec.y;
	b=vec.z;
	a=vec.w;
}

void Color::Apply() const{
	glColor4f(r,g,b,a);
}

