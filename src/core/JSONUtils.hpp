#pragma once

#include <stdint.h>
#include "String.hpp"
#include "Logger.hpp"
#include "b2_math.h"
#include "b2_fixture.h"

#ifdef Bool //Xlib.h troubles
#undef Bool
#endif

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "error/en.h"

namespace jsonutils{

typedef rapidjson::Document::AllocatorType Allocator;
typedef rapidjson::Document::StringRefType StringType;
typedef rapidjson::Document::MemberIterator Iterator;
typedef rapidjson::Document::ConstMemberIterator ConstIterator;
using rapidjson::Value;

Value ToJSON(const b2Vec2 &data, Allocator &allocator);
Value ToJSON(const b2Filter &data, Allocator &allocator);
Value ToJSON(const String &data);

template<class T>
inline Value ToJSON(const T &object, Allocator &allocator){
	Value value;
	object.ToJSON(value, allocator);
	return value;
}

bool FromJSON(const Value &value, bool &data);
bool FromJSON(const Value &value, uint8_t &data);
bool FromJSON(const Value &value, uint16_t &data);
bool FromJSON(const Value &value, uint32_t &data);
bool FromJSON(const Value &value, uint64_t &data);
bool FromJSON(const Value &value, int8_t &data);
bool FromJSON(const Value &value, int16_t &data);
bool FromJSON(const Value &value, int32_t &data);
bool FromJSON(const Value &value, int64_t &data);
bool FromJSON(const Value &value, float &data);
bool FromJSON(const Value &value, double &data);
bool FromJSON(const Value &value, b2Vec2 &data);
bool FromJSON(const Value &value, b2Filter &data);
bool FromJSON(const Value &value, String &data);
bool FromJSON(const Value &value, const char *&data);

template<class T>
inline bool FromJSON(const Value &value, T &object){
	return object.FromJSON(value);
}

bool CheckValue(const Value &parent, const char *name);

bool CheckObject(const Value &value);

bool CheckArray(const Value &array, size_t size);
bool CheckArray(const Value &array, size_t minSize, size_t maxSize);

int GetEnum(const char *str, const char **values, size_t count);

template<typename T>
bool GetMember(const Value &parent, const char *name, T &data){
	if(!CheckValue(parent, name))
		return false;
	const Value &value = parent[name];
	if(!FromJSON(value, data)){
		Log(LEVEL_ERROR, "Value \"%s\" has wrong type", name);
		return false;
	}
	return true;
}
template<typename T>
bool GetArrayMember(const Value &parent, size_t n, T &data){
	const Value &value = parent[n];
	if(!FromJSON(value, data)){
		Log(LEVEL_ERROR, "Value of index %s has wrong type", n);
		return false;
	}
	return true;
}


} //namespace jsonutils
  //
