#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

class String {
public:
	typedef char* Iterator;
	typedef const char* ConstIterator;

	String() : str(nullptr) {};
	String(const String &src){
		Allocate(src.str);
	}
	String(const char *src){
		Allocate(src);
	}
	~String(){
		Clear();
	}
	void operator=(const String &src){
		Clear();
		Allocate(src);
	}
	void operator=(const char *src){
		Clear();
		Allocate(src);
	}
	inline char operator[](size_t pos) const{
		return str[pos];
	}
	inline char &operator[](size_t pos){
		return str[pos];
	}
	inline size_t Size() const{
		return strlen(str);
	}
	inline operator const char*() const{
		return str;
	}
	inline const char *c_str() const{
		return str;
	}
	bool operator==(const char *other) const{
		return str && other && strcmp(str, other) == 0;
	}
	bool operator==(const String &other) const{
		return str && other.str && strcmp(str, other.str) == 0;
	}
	bool IsEmpty() const{
		return str[0] == '\0';
	}
	void Clear(){
		if(str)
			free(str);
	}
	static String Format(const char *format, ...){
		va_list args;
		va_start(args, format);
		char buf[1024];
		vsnprintf(buf, 1024, format, args);
		va_end(args);
		return String(buf);
	}
private:
	void Allocate(const char *src){
		str = (char*)malloc(strlen(src) + 1);
		strcpy(str, src);
	}
	char *str;
};
