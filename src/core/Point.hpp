#pragma once
#include "JSONUtils.hpp"

template<typename T>
struct Point {
	Point() {}
	Point(T _x, T _y) : x(_x), y(_y) {}

	void Set(T _x, T _y){
		x = _x;
		y = _y;
	}

	T x;
	T y;

	void ToJSON(jsonutils::Value &value, jsonutils::Allocator &allocator) const{
		value.SetArray();
		value.PushBack(x, allocator);
		value.PushBack(y, allocator);
	}
	bool FromJSON(const rapidjson::Value &value){
		return
			jsonutils::CheckArray(value, 2) &&
			jsonutils::GetArrayMember(value, 0, x) &&
			jsonutils::GetArrayMember(value, 1, y);
	}
};
	
typedef Point<int> IntPoint;
