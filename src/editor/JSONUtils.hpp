#pragma once

#include <stdint.h>
#include <wx/string.h>
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
typedef rapidjson::Document::StringRefType String;
using rapidjson::Value;

Value ToJSON(const b2Vec2 &data, Allocator &allocator);
Value ToJSON(const b2Filter &data, Allocator &allocator);
Value ToJSON(const wxString &data);

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
bool FromJSON(const Value &value, wxString &data);
bool FromJSON(const Value &value, const char *&data);

template<typename T>
bool GetMember(const Value &parent, const char *name, T &data){
	if(!parent.IsObject() || !parent.HasMember(name))
		return false;
	const Value &value = parent[name];
	return FromJSON(value, data);
}
template<typename T>
bool GetMember(const Value &parent, const char *name, const char **values, size_t count, T &data){
	const char *str;
	if(!GetMember(parent, name, str))
		return false;
	for(int i = 0; i < count; i++)
		if(strcmp(values[i], str) == 0){
			data = (T)i;
			return true;
		}
	return false;
}

} //namespace jsonutils
