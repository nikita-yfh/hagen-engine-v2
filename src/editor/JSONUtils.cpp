#include "JSONUtils.hpp"
#include <limits.h>

namespace jsonutils{

Value ToJSON(const b2Vec2 &data, Allocator &allocator){
	Value value(rapidjson::kArrayType);
	value.PushBack(data.x, allocator);
	value.PushBack(data.y, allocator);
	return value;
}
Value ToJSON(const b2Filter &data, Allocator &allocator){
	Value value(rapidjson::kObjectType);
	value.AddMember("category",	data.categoryBits,	allocator);
	value.AddMember("mask",		data.maskBits,		allocator);
	value.AddMember("group",	data.groupIndex,	allocator);
	return value;
}
Value ToJSON(const wxString &data){
	return Value(String(data.mb_str().data()));
}

bool FromJSON(const Value &value, bool &data){
	if(!value.IsBool())
		return false;
	data = value.GetBool();
	return true;
}
bool FromJSON(const Value &value, uint8_t &data){
	if(!value.IsUint() || value.GetUint() > UINT8_MAX)
		return false;
	data = value.GetUint();
	return true;
}
bool FromJSON(const Value &value, uint16_t &data){
	if(!value.IsUint() || value.GetUint() > UINT16_MAX)
		return false;
	data = value.GetUint();
	return true;
}
bool FromJSON(const Value &value, uint32_t &data){
	if(!value.IsUint())
		return false;
	data = value.GetUint();
	return true;
}
bool FromJSON(const Value &value, uint64_t &data){
	if(!value.IsUint64())
		return false;
	data = value.GetUint64();
	return true;
}
bool FromJSON(const Value &value, int8_t &data){
	if(!value.IsInt() || value.GetInt() < INT8_MIN || value.GetInt() > INT8_MAX)
		return false;
	data = value.GetInt();
	return true;
}
bool FromJSON(const Value &value, int16_t &data){
	if(!value.IsInt() || value.GetInt() < INT16_MIN || value.GetInt() > INT16_MAX)
		return false;
	data = value.GetInt();
	return true;
}
bool FromJSON(const Value &value, int32_t &data){
	if(!value.IsInt())
		return false;
	data = value.GetInt();
	return true;
}
bool FromJSON(const Value &value, int64_t &data){
	if(!value.IsInt64())
		return false;
	data = value.GetInt64();
	return true;
}
bool FromJSON(const Value &value, float &data){
	if(!value.IsFloat())
		return false;
	data = value.GetFloat();
	return true;
}
bool FromJSON(const Value &value, double &data){
	if(!value.IsDouble())
		return false;
	data = value.GetDouble();
	return true;
}
bool FromJSON(const Value &value, b2Vec2 &data){
	if(!value.IsArray() || value.Size() !=2 || !value[0].IsNumber() || !value[1].IsNumber())
		return false;
	data.Set(value[0].GetFloat(), value[1].GetFloat());
	return true;
}
bool FromJSON(const Value &value, b2Filter &data){
	if(!value.IsObject())
		return false;
	return
		GetMember(value, "category", data.categoryBits) &&
		GetMember(value, "mask", data.maskBits) &&
		GetMember(value, "group", data.groupIndex);

}
bool FromJSON(const Value &value, wxString &data){
	if(!value.IsString())
		return false;
	data = value.GetString();
	return true;
}
bool FromJSON(const Value &value, const char *&data){
	if(!value.IsString())
		return false;
	data = value.GetString();
	return true;
}

}

