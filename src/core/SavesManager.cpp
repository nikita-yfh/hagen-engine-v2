#include "SavesManager.hpp"
#include "Logger.hpp"
#include "Utility.hpp"

void SavesManager::Set(const char *gameName) {
	#ifdef LOCAL_SAVES
	#ifdef ANDROID
	saves = SDL_AndroidGetExternalStoragePath();
	#else
	saves = "./saves";
	#endif
	#else
	#ifdef ANDROID
	saves = SDL_AndroidGetInternalStoragePath();
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
	saves = res+"/hagen/"+gameName;*/
	#elif defined(__linux__)
	const char* homeEnv = getenv("HOME");
	saves = String::Format("%s/.local/share/hagen/%s", homeEnv, gameName);
	#elif defined(__APPLE__)
	const char* homeEnv = getenv("HOME");
	saves = String::Format("%s/Library/Application Support/hagen/%s", homeEnv, gameName);
	#endif
	#endif
	LogF(LEVEL_INFO, "Saves directory set to \"%s\"", saves.c_str());
	CreateDirectory(saves);
};
bool SavesManager::CreateDirectory(const char *dir) const {
	if(isdir(dir))
		return true;
	if(mkpath(dir, 0755)) {
		LogF(LEVEL_ERROR, "Failed to create directory \"%s\"", dir);
		return false;
	}
	LogF(LEVEL_INFO, "Created directory \"%s\"", dir);
	return true;
}
SDL_RWops *SavesManager::OpenFile(const char *file, const char *mode) const{
	SDL_RWops *rw = SDL_RWFromFile(String::Format("%s/%s", saves.c_str(), file), mode);
	if(!rw)
		LogF(LEVEL_ERROR, "Failed to open file \"%s\": %s", file, SDL_GetError());
	return rw;
}
bool SavesManager::SaveResource(const char *file, const Resource *res) const{
	LogF(LEVEL_INFO, "Saving file \"%s\"", file);
	SDL_RWops *rw = OpenFile(file, "wb");
	if(!rw)
		return false;
	res->Save(rw);
	SDL_RWclose(rw);
	return true;
}
bool SavesManager::LoadResource(const char *file, Resource *res) const{
	LogF(LEVEL_INFO, "Loading file \"%s\"", file);
	SDL_RWops *rw = OpenFile(file, "rb");
	if(!rw)
		return false;
	bool ret = res->Load(rw);
	SDL_RWclose(rw);
	return ret;
}
