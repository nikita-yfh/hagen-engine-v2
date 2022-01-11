#pragma once

#include "Resource.hpp"


class Archive : public JSONResource{
public:

	Archive();
	virtual ~Archive();

	int SaveString(const char *key, const char *val);
	int LoadString(const char *key, char *val) const;
	int LoadString(const char *key, char *val, size_t length) const;

	int SaveStringArray(const char *key, const char *const *arr); 
	int LoadStringArray(const char *key, char **arr) const; 
	int LoadStringArray(const char *key, char **arr, size_t length) const; 

	template<typename T>
	int SaveValue(const char *key, const T &val){
		rapidjson::Value j_key(key,GetAllocator());
		rapidjson::Value j_val(val);
		GetValue().AddMember(j_key,j_val,GetAllocator());
		return 0;
	}
	template<typename T>
	int LoadValue(const char *key, T val) const{
		if(!GetValue().HasMember(key))
			return 1;
		const rapidjson::Value &j_val(GetValue()[key]);
		if(!j_val.Is<T>())
			return 1;
		val=j_val.Get<T>();
		return 0;
	}
	template<typename T>
	int SaveArray(const char *key, const T *arr){
		rapidjson::Value j_arr;
		j_arr.SetArray();
		while(*arr){
			rapidjson::Value j_val(*(arr++));
			j_arr.PushBack(j_val,GetAllocator());
		}
		rapidjson::Value j_key(key,GetAllocator());
		GetValue().AddMember(j_key,j_arr,GetAllocator());
		return 0;
	}
	template<typename T>
	int LoadArray(const char *key, T *arr) const{
		if(!GetValue().HasMember(key))
			return 1;
		const rapidjson::Value &j_arr=GetValue()[key];
		if(!j_arr.IsArray())
			return 1;
		if(arr!=nullptr)
			free(arr);
		size_t size=j_arr.Size();
		arr=(T*)malloc(size+1);
		arr[size]=nullptr;

		for(const rapidjson::Value &j_val : j_arr.GetArray()){
			if(!j_val.Is<T>())
				return 1;
			(*arr++)=j_val.Get<T>();
		}
	}

	template<class T>
	int SaveObject(const char *key, const T *obj){
		return
			PushSaveObject() ||
			obj->SaveJSON(this) ||
			PopSaveObject(key);
	}
	template<class T>
	int LoadObject(const char *key, T *obj){
		return
			PushLoadObject(key) ||
			obj->LoadJSON(this) ||
			PopLoadObject();
	}

	int PushSaveObject();
	int PopLoadObject();
	int PopSaveObject(const char *key);
	int PushLoadObject(const char *key);

protected:
	typedef rapidjson::Document::AllocatorType allocator_t;
	inline allocator_t &GetAllocator(){
		return document.GetAllocator();
	}
	inline rapidjson::Value &GetValue(){
		return *value->value;
	}
	inline const rapidjson::Value &GetValue() const{
		return *value->value;
	}

	struct _Value{
		_Value(rapidjson::Value *_value, _Value *_parent=nullptr)
			:value(_value), parent(_parent){}
		_Value *parent;
		rapidjson::Value *value;
	};
	_Value *value;
};
