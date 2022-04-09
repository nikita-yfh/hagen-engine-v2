#include "ResourceManager.hpp"

#include <sys/stat.h>

#include "Logger.hpp"
#include "Texture.hpp"
#include "Locale.hpp"
#include "Utility.hpp"

ResourceManager::~ResourceManager(){
	ClearResources();
	ClearStorages();
}

bool ResourceManager::AddStorage(const char *path) {
	RemoveStorage(path);
	Storage *res = nullptr;
	if(Directory::Is(path))
		res = new Directory(path);
	else if(ZIPFile::Is(path))
		res = new ZIPFile(path);
	else {
		LogF(LEVEL_WARNING, "Resource \"%s\" not found", path);
		return false;
	}

	if(res->Open()){
		delete res;
		return false;
	}

	res->next = storages;
	storages = res;

	LogF(LEVEL_INFO, "Loaded \"%s\"", path);
	return true;
}
bool ResourceManager::RemoveStorage(const char *path) {
	Storage *del = nullptr;
	if(storages && storages->GetPath() == path){
		del = storages;
		storages = storages->next;
	}else{
		for(Storage *storage = storages; storage; storage = storage->next){
			Storage *next = storage->next;
			if(next && next->GetPath() == path){
				storage->next = next->next;
				break;
			}
		}
	}
	if(!del)
		return false;
	delete del;
	return true;
}
void ResourceManager::ClearStorages() {
	while(storages){
		Storage *next = storages->next;
		delete storages;
		storages = next;
	}

	Log(LEVEL_INFO, "Cleared all storages");
}
bool ResourceManager::HasStorages() const {
	return storages != nullptr;
}
bool ResourceManager::LoadResource(const char *path, Resource *res){
	LogF(LEVEL_INFO, "Loading file \"%s\"", path);
	SDL_RWops *rw = OpenFile(path);
	if(!rw)
		return false;
	bool ret = res->Load(rw);
	SDL_RWclose(rw);
	return ret;
}
void ResourceManager::AddResource(const char *path, Resource *res){
	ResourceEntry *entry = new ResourceEntry;
	entry->next = resources;
	entry->path = path;
	entry->resource = res;
	resources = entry;
}
Resource *ResourceManager::FindResource(const char *path) const{
	for(ResourceEntry *resource = resources; resource; resource = resource->next)
		if(resource->path == path)
			return resource->resource;
	return nullptr;
}
void ResourceManager::ClearResources(){
	while(resources){
		ResourceEntry *next = resources->next;
		delete resources;
		resources = next;
	}
}
SDL_RWops *ResourceManager::OpenFile(const char *path) {
	for(Storage *storage = storages; storage; storage = storage->next)
		if(storage->ExistFile(path))
			return storage->OpenFile(path);
	LogF(LEVEL_ERROR, "Failed to open file \"%s\": file not found", path);
	return nullptr;
}
