#include "JSONUtils.hpp"
#include <limits.h>

namespace jsonutils{

Value ToJSON(const b2Vec2 &data, Allocator &allocator){
	Value value(rapidjson::kArrayType);
	value.PushBack(data.x, allocator);
	value.PushBack(data.y, allocator);
	return value;
}
Value ToJSON(const ImVec2 &data, Allocator &allocator){
	Value value(rapidjson::kArrayType);
	value.PushBack(data.x, allocator);
	value.PushBack(data.y, allocator);
	return value;
}
Value ToJSON(const ImVec4 &data, Allocator &allocator){
	Value value(rapidjson::kArrayType);
	value.PushBack(data.x, allocator);
	value.PushBack(data.y, allocator);
	value.PushBack(data.z, allocator);
	value.PushBack(data.w, allocator);
	return value;
}
Value ToJSON(const b2Filter &data, Allocator &allocator){
	Value value(rapidjson::kObjectType);
	value.AddMember("category",	data.categoryBits,	allocator);
	value.AddMember("mask",		data.maskBits,		allocator);
	value.AddMember("group",	data.groupIndex,	allocator);
	return value;
}
Value ToJSON(const String &data){
	return Value(StringType(data.c_str()));
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
	return
		CheckArray(value, 2) &&
		GetArrayMember(value, 0, data.x) &&
		GetArrayMember(value, 1, data.y);
}
bool FromJSON(const Value &value, ImVec2 &data){
	return
		CheckArray(value, 2) &&
		GetArrayMember(value, 0, data.x) &&
		GetArrayMember(value, 1, data.y);
}
bool FromJSON(const Value &value, ImVec4 &data){
	return
		CheckArray(value, 4) &&
		GetArrayMember(value, 0, data.x) &&
		GetArrayMember(value, 1, data.y) &&
		GetArrayMember(value, 2, data.z) &&
		GetArrayMember(value, 3, data.w);
}
bool FromJSON(const Value &value, b2Filter &data){
	return
		CheckObject(value) &&
		GetMember(value, "category", data.categoryBits) &&
		GetMember(value, "mask", data.maskBits) &&
		GetMember(value, "group", data.groupIndex);

}
bool FromJSON(const Value &value, String &data){
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
bool CheckValue(const Value &parent, const char *name, bool required){
	if(!parent.IsObject() || !parent.HasMember(name)){
		LogF(required ? LEVEL_ERROR : LEVEL_WARNING, "Value \"%s\" not found", name);
		return false;
	}
	return true;
}

bool CheckObject(const Value &value){
	if(!value.IsObject()){
		Log(LEVEL_ERROR, "Value is not an object");
		return false;
	}
	return true;
}

bool CheckArray(const Value &array) {
	if(!array.IsArray()){
		Log(LEVEL_ERROR, "Value is not an array");
		return false;
	}
	return true;
}
	
bool CheckArray(const Value &array, size_t size) {
	if(!CheckArray(array))
		return false;
	if(array.Size() != size){
		LogF(LEVEL_ERROR, "Wrong array size (size %zu, need %zu)", array.Size(), size);
		return false;
	}
	return true;
}
bool CheckArray(const Value &array, size_t minSize, size_t maxSize) {
	if(!CheckArray(array))
		return false;
	if(array.Size() < minSize || array.Size() > maxSize){
		LogF(LEVEL_ERROR, "Wrong array size (size %zu, need %zu-%zu)",
							array.Size(), minSize, maxSize);
		return false;
	}
	return true;
}

int GetEnum(const char *str, const char **values, size_t count){
	for(int i = 0; i < count; i++)
		if(strcmp(values[i], str) == 0)
			return i;
	LogF(LEVEL_ERROR, "String \"%s\" does not match avaliable strings", str);
	return -1;
}

}

