#pragma once

#include <stdint.h>
#include <limits.h>
#include <wx/string.h>
#include "b2_math.h"

#ifdef Bool //Xlib.h troubles
#undef Bool
#endif

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "error/en.h"

namespace rapidjson{
namespace internal{

template<typename ValueType> 
struct TypeHelper<ValueType, int8_t> {
    static inline bool Is(const ValueType& v) { return v.IsInt() && v.GetInt() >= INT8_MIN && v.GetInt() <= INT8_MAX; }
    static inline int8_t Get(const ValueType& v) { return v.GetInt(); }
    static inline ValueType& Set(ValueType& v, int8_t data) { return v.SetInt(data); }
    static inline ValueType& Set(ValueType& v, int8_t data, typename ValueType::AllocatorType&) { return v.SetInt(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, uint8_t> {
    static inline bool Is(const ValueType& v) { return v.IsUint() && v.GetUint() <= UINT8_MAX; }
    static inline uint8_t Get(const ValueType& v) { return v.GetUint(); }
    static inline ValueType& Set(ValueType& v, uint8_t data) { return v.SetUint(data); }
    static inline ValueType& Set(ValueType& v, uint8_t data, typename ValueType::AllocatorType&) { return v.SetUint(data); }
};
template<typename ValueType> 
struct TypeHelper<ValueType, int16_t> {
    static inline bool Is(const ValueType& v) { return v.IsInt() && v.GetInt() >= INT16_MIN && v.GetInt() <= INT16_MAX; }
    static inline int16_t Get(const ValueType& v) { return v.GetInt(); }
    static inline ValueType& Set(ValueType& v, int16_t data) { return v.SetInt(data); }
    static inline ValueType& Set(ValueType& v, int16_t data, typename ValueType::AllocatorType&) { return v.SetInt(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, uint16_t> {
    static inline bool Is(const ValueType& v) { return v.IsUint() && v.GetUint() <= UINT16_MAX; }
    static inline uint16_t Get(const ValueType& v) { return v.GetUint(); }
    static inline ValueType& Set(ValueType& v, uint16_t data) { return v.SetUint(data); }
    static inline ValueType& Set(ValueType& v, uint16_t data, typename ValueType::AllocatorType&) { return v.SetUint(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, wxString> {
	static bool Is(const ValueType& v) { return v.IsString(); }
	static wxString Get(const ValueType& v) { return v.GetString(); }
	static ValueType& Set(ValueType& v, const wxString &data) { return v.SetString(typename ValueType::StringRefType(data.mb_str().data())); }
	static ValueType& Set(ValueType& v, const wxString &data, typename ValueType::AllocatorType& a) { return v.SetString(data.mb_str().data(), a); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, b2Vec2> {
	static bool Is(const ValueType& v) { return v.IsArray() && v.Size() == 2 && v[0].IsNumber() && v[1].IsNumber(); }
	static b2Vec2 Get(const ValueType& v) { return b2Vec2(v[0].GetFloat(), v[1].GetFloat()); }
	static ValueType& Set(ValueType& v, const b2Vec2 &data, typename ValueType::AllocatorType& a) {
		v.SetArray();
		v.PushBack(data.x, a);
		v.PushBack(data.y, a);
		return v;
	}
};

}
}

namespace jsonutils{

typedef rapidjson::Document::AllocatorType Allocator;
typedef rapidjson::Document::StringRefType String;

template<typename T>
inline bool Is(const rapidjson::Value &v) {
	return rapidjson::internal::TypeHelper<rapidjson::Value, T>::Is(v);
}
template<typename T>
inline T Get(const rapidjson::Value &v) {
	return rapidjson::internal::TypeHelper<rapidjson::Value, T>::Get(v);
}
template<typename T>
inline void Set(rapidjson::Value& v, const T &data, Allocator &a) {
	rapidjson::internal::TypeHelper<rapidjson::Value, T>::Set(v, data, a);
}
template<typename T>
inline void Set(rapidjson::Value& v, const T &data) {
	rapidjson::internal::TypeHelper<rapidjson::Value, T>::Set(v, data);
}

template<typename T>
rapidjson::Value Value(const T &data){
	rapidjson::Value value;
	Set<T>(value, data);
	return value;
}
template<typename T>
rapidjson::Value Value(const T &data, Allocator &allocator){
	rapidjson::Value value;
	Set<T>(value, data, allocator);
	return value;
}

template<typename T>
bool GetMember(const rapidjson::Value &parent, const char *name, T &data){
	if(!parent.IsObject() || !parent.HasMember(name))
		return false;
	const rapidjson::Value &value = parent[name];
	if(!Is<T>(value))
		return false;
	data = Get<T>(value);
	return true;
}
template<typename T>
bool GetMember(const rapidjson::Value &parent, const char *name, const char **values, size_t count, T &data){
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
