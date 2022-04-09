#pragma once

#include "SDL.h"
#include "Resource.hpp"
#include "Logger.hpp"
#include "String.hpp"

class SavesManager{
public:
	void Set(const char *gameName);

	bool LoadResource(const char *file, Resource *res) const;
	bool SaveResource(const char *file, const Resource *res) const;

	template<class T>
	T *LoadResource(const char *path){
		T *resource = new T();
		if(LoadResource(path, resource))
			return resource;
		delete resource;
		return nullptr;
	}

	template<class T>
	bool LoadJSON(const char *path, T &object){
		JSONResource res;
		return
			LoadResource(path, &res) &&
			res.FromJSON(object);
	}
	template<class T>
	bool SaveJSON(const char *path, const T &object){
		JSONResource res;
		res.ToJSON(object);
		return SaveResource(path, &res);
	}
private:
	SDL_RWops *OpenFile(const char *file, const char *mode) const;
	bool CreateDirectory(const char *dir) const;

	String saves;
};
