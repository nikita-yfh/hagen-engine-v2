#pragma once

#include "SDL.h"
#include "Resource.hpp"
#include "Serializer.hpp"
#include "Logger.hpp"

class SavesManager{
public:
	SavesManager(const char *gameName);
	~SavesManager();


	template<class T>
	int LoadJSON(const char *file, T *archivable) const{
		Archive archive;
		if(LoadResource(file, &archive))
			return 1;
		return archivable->LoadJSON(&archive);
	}

	template<typename T>
	int SaveJSON(const char *file, const T *archivable) const{
		Archive archive;
		return archivable->SaveJSON(&archive) ||
				SaveResource(file,&archive);
	}
	int LoadResource(const char *file, Resource *res) const;
	int SaveResource(const char *file, const Resource *res) const;
private:
	SDL_RWops *OpenFile(const char *file,const char *mode) const;
	int CreateDirectory(const char *dir) const;

	char *saves;
};
