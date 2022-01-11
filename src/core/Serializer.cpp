#include "Serializer.hpp"

Archive::Archive(){
	document.SetObject();
}
Archive::~Archive(){
	while(value){
		_Value *v=value->parent;
		delete value;
		value=v;
	}
}

int Archive::SaveString(const char *key, const char *val){
	rapidjson::Value j_key(key,GetAllocator());
	rapidjson::Value j_val;
	j_val.SetString(val,GetAllocator());
	GetValue().AddMember(j_key,j_val,GetAllocator());
	return 0;
}
int Archive::LoadString(const char *key, char *val) const{
	if(!GetValue().HasMember(key))
		return 1;
	const rapidjson::Value &j_val(GetValue()[key]);
	if(!j_val.IsString())
		return 1;
	if(val != nullptr)
		free(val);
	size_t length=j_val.GetStringLength()+1;
	val=(char*)malloc(length);
	memcpy(val,j_val.GetString(),length);
	return 0;
}
int Archive::LoadString(const char *key, char *val, size_t length) const{
	if(!GetValue().HasMember(key))
		return 1;
	const rapidjson::Value &j_val(GetValue()[key]);
	if(!j_val.IsString())
		return 1;
	if(val == nullptr)
		return 1;
	strncpy(val,j_val.GetString(), length);
	return 0;
}
int Archive::SaveStringArray(const char *key, const char *const *arr){
	rapidjson::Value j_arr;
	j_arr.SetArray();
	while(*(arr++)){
		const char *val=*arr;
		rapidjson::Value j_val(val,GetAllocator());
		j_arr.PushBack(j_val,GetAllocator());
	}
	rapidjson::Value j_key(key,GetAllocator());
	GetValue().AddMember(j_key,j_arr,GetAllocator());
	return 0;
}
int Archive::LoadStringArray(const char *key, char **arr) const{
	if(!GetValue().HasMember(key))
		return 1;
	const rapidjson::Value &j_arr=GetValue()[key];
	if(!j_arr.IsArray())
		return 1;
	if(arr!=nullptr){
		char **ptr=arr;
		while(*ptr) free(*(ptr++));
		free(arr);
	}
	size_t size=j_arr.Size();
	arr=(char**)malloc(size+1);
	arr[size]=nullptr;

	for(const rapidjson::Value &j_val : j_arr.GetArray()){
		if(!j_val.IsString())
			return 1;
		size_t size=j_val.GetStringLength()+1;
		(*arr)=(char*)malloc(size);
		memcpy((*arr++),j_val.GetString(),size);
	}
	return 0;
}
int Archive::LoadStringArray(const char *key, char **arr, size_t len) const{
	if(!GetValue().HasMember(key))
		return 1;
	const rapidjson::Value &j_arr=GetValue()[key];
	if(!j_arr.IsArray())
		return 1;
	if(arr!=nullptr)
		free(arr);
	size_t size=j_arr.Size();
	arr=(char**)malloc(size+1);
	arr[size]=nullptr;

	for(const rapidjson::Value &j_val : j_arr.GetArray()){
		if(!j_val.IsString())
			return 1;
		strncpy(*(arr++),j_val.GetString(),len);
	}
	return 0;
}
int Archive::PushSaveObject(){
	value=new _Value(new rapidjson::Value(),value);
	GetValue().SetObject();
	return 0;
}
int Archive::PopLoadObject(){
	_Value *parent=value->parent;
	if(parent==nullptr)
		return 1;
	delete value;
	value=parent;
	return 0;
}

int Archive::PopSaveObject(const char *key){
	_Value *parent=value->parent;
	if(parent==nullptr)
		return 1;
	rapidjson::Value j_key(key,GetAllocator());
	parent->value->AddMember(j_key,*value->value,GetAllocator());
	delete value;
	value=parent;
	return 0;
}
int Archive::PushLoadObject(const char *key){
	if(!GetValue().HasMember(key))
		return 1;
	rapidjson::Value &j_value=GetValue()[key];
	value=new _Value(&j_value,value);
	return 0;
}
