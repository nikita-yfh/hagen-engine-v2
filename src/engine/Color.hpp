#pragma once

#include "imgui.h"
#include "JSONUtils.hpp"

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

	rapidjson::Value ToJSON(jsonutils::Allocator &allocator) const;
	bool FromJSON(const rapidjson::Value &value);
};
