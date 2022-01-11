#pragma once

#include "imgui.h"
#include "Serializer.hpp"

class Color{
public:
	Color(const ImVec4&);
	Color(float _r,float _g,float _b,float _a)
		: r(_r),g(_g),b(_b),a(_a) {}

	operator ImVec4() const{
		return ImVec4(r,g,b,a);
	}

	void Apply() const;

	float r;
	float g;
	float b;
	float a;

	int SaveJSON(Archive *archive) const;
	int LoadJSON(Archive *archive);

};
