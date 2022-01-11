#pragma once

#include "SDL.h"
#include "miniz.h"
#include "Logger.hpp"
#include "Resource.hpp"
#include "Serializer.hpp"

struct _Resource{
	char *path;
	Resource *resource;
	_Resource *next;
};

class Storage {
public:
	Storage(const char *path);
	virtual ~Storage();

	virtual int Init() = 0;
	virtual int Quit() = 0;

	virtual SDL_RWops *OpenFile(const char *file) = 0;
	virtual bool ExistFile(const char *file) = 0;

protected:
	char *path;
private:
	Storage *next;
	friend class ResourceManager;
};

class Directory : public Storage {
public:
	Directory(const char *path) : Storage(path) {}

	int Init();
	int Quit();

	SDL_RWops *OpenFile(const char *file);
	bool ExistFile(const char *file);

	static bool Is(const char *path);
private:
	SDL_RWops *Open(const char *file);
};


class ZIPFile : public Storage {
public:
	ZIPFile(const char *path) : Storage(path), data(nullptr), dataSize(0) {}

	int Init();
	int Quit();

	SDL_RWops *OpenFile(const char *path);
	bool ExistFile(const char *path);

	static bool Is(const char *path);
private:
	static const char *ErrorToString(mz_zip_error);

	void *data;
	size_t dataSize;

	mz_zip_archive zip;
};

class ResourceManager {
public:
	ResourceManager(const char*const*storages,size_t num);
	~ResourceManager();

	int AddStorage(const char *path);
	int RemoveStorage(const char *path);
	bool HasStorages() const;
	void ClearStorages();

	template<class T>
	int LoadJSON(const char *path, T *archivable){
		Archive archive;
		return LoadResource(path,&archive) ||
			archivable->LoadJSON(&archive);
	}

	int LoadResource(const char *path, Resource *res);

	void AddResource(const char *path,Resource *res);
	void ClearResources();
private:
	Resource *FindResource(const char *path) const;
	void AddResource(Resource *res);
	SDL_RWops *OpenFile(const char *path);

	Storage *storages;

	_Resource *resources;
};
