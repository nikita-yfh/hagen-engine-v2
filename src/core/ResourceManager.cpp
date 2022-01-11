#include "ResourceManager.hpp"

#include <sys/stat.h>

#include "Logger.hpp"
#include "Texture.hpp"
#include "Locale.hpp"
#include "Utility.hpp"

ResourceManager::ResourceManager(const char*const*pathes,size_t num)
		:storages(nullptr), resources(nullptr) {
	for(int q=0;q<num;q++)
		AddStorage(pathes[q]);

	if(!HasStorages())
		Log(LEVEL_FATAL,"No resources to load! Exiting...");
}

ResourceManager::~ResourceManager(){
	ClearResources();
	ClearStorages();
}

int ResourceManager::AddStorage(const char *path) {
	RemoveStorage(path);
	Storage *res=nullptr;
	if(Directory::Is(path))
		res=new Directory(path);
	else if(ZIPFile::Is(path))
		res=new ZIPFile(path);
	else {
		Log(LEVEL_WARNING,"Resource \"%s\" not found",path);
		return 1;
	}

	if(res->Init()){
		delete res;
		return 1;
	}

	if(storages==nullptr)
		storages=res;
	else{
		res->next=storages;
		storages=res;
	}

	Log(LEVEL_INFO,"Loaded \"%s\"",path);
	return 0;
}
int ResourceManager::RemoveStorage(const char *path) {
	for(Storage *storage=storages;storage;storage=storage->next){
		if(strcmp(storage->next->path,path)==0){
			Storage *temp=storage->next->next;
			delete storage->next;
			storage->next=temp;
			return 0;
		}
	}
	return 1;
}
void ResourceManager::ClearStorages() {
	Storage *storage=storages;
	while(storage){
		Storage *temp=storage->next;
		delete storage;
		storage=temp;
	}

	Log(LEVEL_INFO,"Cleared all storages");
}
bool ResourceManager::HasStorages() const {
	return storages!=nullptr;
}
int ResourceManager::LoadResource(const char *path, Resource *res){
	SDL_RWops *rw=OpenFile(path);
	if(rw==nullptr)
		return 1;
	int ret=res->Save(rw);
	SDL_RWclose(rw);
	return ret;
}
void ResourceManager::AddResource(const char *path,Resource *res){
	_Resource *_res=new _Resource;
	_res->next=resources;
	_res->path=cstrdup(path);
	_res->resource=res;
	resources=_res;
}
Resource *ResourceManager::FindResource(const char *path) const{
	for(_Resource *resource=resources;resource;resource=resource->next)
		if(strcmp(resource->path,path)==0)
			return resource->resource;
	return nullptr;
}
void ResourceManager::ClearResources(){
	_Resource *resource=resources;
	while(resource){
		_Resource *temp=resource->next;
		delete resource;
		resource=temp;
	}
}
SDL_RWops *ResourceManager::OpenFile(const char *path) {
	for(Storage *storage=storages;storage;storage=storage->next)
		if(storage->ExistFile(path))
			return storage->OpenFile(path);
	Log(LEVEL_ERROR,"Failed to open file \"%s\": file not found",path);
	return nullptr;
}
//class Storage

Storage::Storage(const char *_path)
		:next(nullptr) {
	path=(char*)malloc(strlen(_path)+1);

	strcpy(path,_path);
}
Storage::~Storage(){
	free(path);
}
//class Directory

int Directory::Init() {
	return 0;
}
int Directory::Quit() {
	return 0;
}
SDL_RWops *Directory::Open(const char *file) {
	char *_path=(char*)malloc(strlen(path)+strlen(file)+2);
	sprintf(_path,"%s/%s",path,file);
	SDL_RWops *rw=SDL_RWFromFile(_path,"rb");
	free(_path);
	return rw;
}
SDL_RWops *Directory::OpenFile(const char *file) {
	SDL_RWops *rw=Open(file);
	if(!rw)
		Log(LEVEL_ERROR,"Failed to open file \"%s\": %s",path,SDL_GetError());
	return rw;
}
bool Directory::ExistFile(const char *file) {
	SDL_RWops *rw=Open(file);
	if(rw==nullptr)
		return false;
	SDL_RWclose(rw);
	return true;
}
bool Directory::Is(const char *path) {
#ifdef ANDROID
	if(strcmp(path,"")==0)
		return true; //assets path must be empty for Android
#endif
	struct stat info;
	if(stat(path, &info) == 0 && info.st_mode & S_IFDIR)
		return true;
	return false;
}

//class ZIPFile

int ZIPFile::Init() {
	SDL_RWops *rw=SDL_RWFromFile(path,"rb");
	if(!rw){
		Log(LEVEL_ERROR,"Failed to load file \"%s\": %s",
			path,SDL_GetError());
		return 1;
	}
	TextResource textRes;

	if(!textRes.Load(rw)){
		SDL_RWclose(rw);
		return 1;
	}
	SDL_RWclose(rw);

	zip= {0}; //init with 0, doesn't work without it

	if(!mz_zip_reader_init_mem(&zip, textRes.data, textRes.size,
			MZ_DEFAULT_LEVEL | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY)) {
		Log(LEVEL_ERROR,"Failed to load archive \"%s\": %s",
			path,ErrorToString(zip.m_last_error));
		return 1;
	}
	return 0;
}
int ZIPFile::Quit() {
	mz_zip_reader_end(&zip);
	free(data);
	return 0;
}
SDL_RWops *ZIPFile::OpenFile(const char *path) {
	void *file;
	size_t fileSize;

	size_t entrylen = 0;

	int entryIndex = mz_zip_reader_locate_file(&zip, path, NULL, 0);
	if(entryIndex==-1) {
		Log(LEVEL_ERROR,"Failed to load file \"%s\": file not found",path);
		return nullptr;
	}

	mz_zip_archive_file_stat stats;
	mz_zip_reader_file_stat(&zip, entryIndex, &stats);
	file = mz_zip_reader_extract_to_heap(&zip, entryIndex, &fileSize, 0);

	SDL_RWops *rw=SDL_RWFromMem(file,fileSize);
	if(!rw)
		Log(LEVEL_ERROR,"Failed to load file \"%s\": %s",path,SDL_GetError());
	return rw;
}
bool ZIPFile::ExistFile(const char *path) {
	int entryIndex = mz_zip_reader_locate_file(&zip, path, NULL, 0);
	return entryIndex!=-1;
}
bool ZIPFile::Is(const char *path) {
	SDL_RWops *rw=SDL_RWFromFile(path,"rb");
	if(rw==nullptr)
		return false;
	SDL_RWclose(rw);
	return true;
}
const char *ZIPFile::ErrorToString(mz_zip_error err) {
	static const char *errors[]= {
		"Undefined error",
		"Too many files",
		"File too large",
		"Unsupported method",
		"Unsupported encription",
		"Unsupported feature",
		"Failed find central dir",
		"Not an archive",
		"Invalid header or corrupted",
		"Unsupported multidisk",
		"Decompression failed",
		"Compression failed",
		"Unexpected_decompressed_size",
		"Crc_check_failed",
		"Unsupported_cdir_size",
		"Alloc_failed",
		"File_open_failed",
		"File_create_failed",
		"File_write_failed",
		"File_read_failed",
		"File_close_failed",
		"File_seek_failed",
		"File_stat_failed",
		"Invalid_parameter",
		"Invalid_filename",
		"Buf_too_small",
		"Internal_error",
		"File_not_found",
		"Archive_too_large",
		"Validation_failed",
		"Write_callback_failed"
	};
	return errors[err];
}
