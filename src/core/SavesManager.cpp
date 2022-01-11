#include "SavesManager.hpp"
#include "Logger.hpp"
#include "Utility.hpp"

SavesManager::SavesManager(const char *gameName) {
	#ifdef LOCAL_SAVES
	#ifdef ANDROID
	saves=cstrdup(SDL_AndroidGetExternalStoragePath());
	#else
	saves=cstrdup("./saves");
	#endif
	#else
	#ifdef ANDROID
	saves=strdup(SDL_AndroidGetInternalStoragePath());
	#elif defined(_WIN32)
	/*LPWSTR wszPath = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &wszPath);
	string res;
	int actualSize = WideCharToMultiByte(CP_UTF8, 0, wszPath, -1, nullptr, 0, nullptr, nullptr);
	if (actualSize > 0) {
		vector<char> buffer(actualSize);
		actualSize = WideCharToMultiByte(CP_UTF8, 0, wszPath, -1, &buffer[0], static_cast<int>(buffer.size()), nullptr, nullptr);
		res = buffer.data();
	}
	saves=res+"/hagen/"+gameName;*/
	#elif defined(__linux__)
	const char* homeEnv = getenv("HOME");
	saves=csprintf("%s/.local/share/hagen/%s",homeEnv,gameName);
	#elif defined(__APPLE__)
	const char* homeEnv = getenv("HOME");
	saves=csprintf("%s/Library/Application Support/hagen/%s",homeEnv,gameName);
	#endif
	#endif
	Log(LEVEL_INFO,"Saves directory set to \"%s\"",saves);
	CreateDirectory(saves);
};

SavesManager::~SavesManager() {
	free(saves);
}

int SavesManager::CreateDirectory(const char *dir) const {
	if(isdir(dir))
		return 0;
	if(mkpath(dir,0755)) {
		Log(LEVEL_ERROR,"Failed to create directory \"%s\"",dir);
		return 1;
	}
	Log(LEVEL_INFO,"Created directory \"%s\"",dir);
	return 0;
}


SDL_RWops *SavesManager::OpenFile(const char *file,const char *mode) const{
	char *path=(char*)malloc(strlen(saves)+strlen(file)+2);
	sprintf(path,"%s/%s",saves,file);
	SDL_RWops *rw=SDL_RWFromFile(path,mode);
	if(!rw)
		Log(LEVEL_ERROR,"Failed to open file \"%s\": %s",file,SDL_GetError());
	return rw;
}
int SavesManager::SaveResource(const char *file, const Resource *res) const{
	Log(LEVEL_INFO,"Saving file \"%s\"",file);
	SDL_RWops *rw=OpenFile(file,"wb");
	if(rw==nullptr)
		return 1;
	res->Save(rw);
	SDL_RWclose(rw);
	return 0;
}
int SavesManager::LoadResource(const char *file, Resource *res) const{
	Log(LEVEL_INFO,"Loading file \"%s\"",file);
	SDL_RWops *rw=OpenFile(file,"rb");
	if(rw==nullptr)
		return 1;
	res->Load(rw);
	SDL_RWclose(rw);
	return 0;
}
