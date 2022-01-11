#include "Resource.hpp"
#include "Logger.hpp"
#include "error/en.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "Utility.hpp"

using namespace rapidjson;

TextResource::TextResource()
	:size(0), data(nullptr) {}
int TextResource::Load(SDL_RWops *file){
	size=SDL_RWsize(file);
	if(AllocateBuffer())
		return 1;

	size_t readTotal = 0, read = 1;
	char* buf = data;
	while (readTotal < size && read != 0) {
		read = SDL_RWread(file, buf, 1, (size - readTotal));
		readTotal += read;
		buf += read;
	}
	if (readTotal != size) {
		Log(LEVEL_ERROR,"Failed to load file: %s",SDL_GetError());
		free(data);
		data=nullptr;
		return 1;
	}
	data[size]='\0'; //null-terminator
	return 0;
}
int TextResource::Load(const char *text){
	size=strlen(text);
	if(AllocateBuffer())
		return 1;
	memcpy(data,text,size);
	return 0;
}

int TextResource::Save(SDL_RWops *rw) const {
	return SDL_RWwrite(rw,data,1,size);
}

int TextResource::AllocateBuffer(){
	data=(char*)malloc(size+1);
	if(data==nullptr){
		Log(LEVEL_ERROR,"Failed to allocate buffer");
		return 1;
	}
	return 0;
}

TextResource::~TextResource(){
	if(data)
		free(data);
}
int JSONResource::Load(SDL_RWops *file){
	TextResource textRes;
	if(textRes.Load(file))
		return 1;
	document.Parse(textRes.data,textRes.size);
	LogError(textRes.data);
	return 0;
}
int JSONResource::Load(const char *text,size_t size){
	document.Parse(text,size);
	return LogError(text);
}
int JSONResource::Load(const char *text){
	document.Parse(text);
	return LogError(text);
}
int JSONResource::Save(SDL_RWops *file) const{
	StringBuffer s;
	PrettyWriter<StringBuffer>writer(s);
	document.Accept(writer);
	TextResource textRes;
	return textRes.Load(s.GetString()) ||
			textRes.Save(file);
}	

JSONResource::JSONResource(){
	document.SetObject();
}
int JSONResource::LogError(const char *text) const{
	if(document.HasParseError()){
		size_t pos=document.GetErrorOffset();
		size_t col=1;
		size_t str=1;
		for(int q=0;q<pos;q++){
			if(text[q]=='\n'){
				col=1;
				str++;
			}else
				col++;
		}
		Log(LEVEL_ERROR,"Failed to parse JSON (string %d, column %d): %s",
			str,col,GetParseError_En(document.GetParseError()));
		return 1;
	}
	return 0;
}
