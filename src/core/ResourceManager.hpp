#pragma once

#include "SDL.h"
#include "miniz.h"
#include "Logger.hpp"
#include "Resource.hpp"
#include "Storage.hpp"
#include "String.hpp"
#include "JSONUtils.hpp"

struct ResourceEntry{
	String path;
	Resource *resource;
	ResourceEntry *next;
};

class ResourceManager {
public:
	~ResourceManager();

	bool AddStorage(const char *path);
	bool RemoveStorage(const char *path);
	bool HasStorages() const;
	void ClearStorages();

	template<class T>
	T *LoadResource(const char *path){
		T *resource = new T();
		if(!LoadResource(path, resource)){
			delete resource;
			return nullptr;
		}
		AddResource(path, resource);
		return resource;
	}

	template<class T>
	bool LoadJSON(const char *path, T &object){
		JSONResource res;
		return
			LoadResource(path, &res) &&
			res.FromJSON(object);
	}
private:
	bool LoadResource(const char *path, Resource *res);
	void AddResource(const char *path, Resource *res);
	void ClearResources();

	Resource *FindResource(const char *path) const;
	SDL_RWops *OpenFile(const char *path);

	Storage *storages;

	ResourceEntry *resources;
};
