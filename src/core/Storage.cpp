#include "Storage.hpp"
#include "Logger.hpp"
#include "Resource.hpp"

#include <sys/stat.h>

// Directory

int Directory::Open() {
	return 0;
}
int Directory::Close() {
	return 0;
}
SDL_RWops *Directory::OpenFile(const char *file) {
	char _path[4096];
	sprintf(_path,"%s/%s", path.c_str(), file);
	SDL_RWops *rw = SDL_RWFromFile(_path, "rb");
	if(!rw)
		Log(LEVEL_ERROR,"Failed to open file \"%s\": %s", file, SDL_GetError());
	return rw;
}
bool Directory::ExistFile(const char *file) {
	SDL_RWops *rw = OpenFile(file);
	if(rw == nullptr)
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

// ZIPFile

int ZIPFile::Open() {
	SDL_RWops *rw = SDL_RWFromFile(path, "rb");
	if(!rw){
		Log(LEVEL_ERROR,"Failed to load file \"%s\": %s",
			path, SDL_GetError());
		return 1;
	}
	TextResource textRes;

	if(!textRes.Load(rw)){
		SDL_RWclose(rw);
		return 1;
	}
	SDL_RWclose(rw);

	zip = {0}; //init with 0, doesn't work without it

	if(!mz_zip_reader_init_mem(&zip, textRes.data, textRes.size,
			MZ_DEFAULT_LEVEL | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY)) {
		Log(LEVEL_ERROR,"Failed to load archive \"%s\": %s",
			path, ErrorToString(zip.m_last_error));
		return 1;
	}
	return 0;
}
int ZIPFile::Close() {
	mz_zip_reader_end(&zip);
	free(data);
	return 0;
}
SDL_RWops *ZIPFile::OpenFile(const char *path) {
	void *file;
	size_t fileSize;

	size_t entrylen = 0;

	int entryIndex = mz_zip_reader_locate_file(&zip, path, NULL, 0);
	if(entryIndex == -1) {
		Log(LEVEL_ERROR,"Failed to load file \"%s\": file not found", path);
		return nullptr;
	}

	mz_zip_archive_file_stat stats;
	mz_zip_reader_file_stat(&zip, entryIndex, &stats);
	file = mz_zip_reader_extract_to_heap(&zip, entryIndex, &fileSize, 0);

	SDL_RWops *rw = SDL_RWFromMem(file, fileSize);
	if(!rw)
		Log(LEVEL_ERROR,"Failed to load file \"%s\": %s", path, SDL_GetError());
	return rw;
}
bool ZIPFile::ExistFile(const char *path) {
	int entryIndex = mz_zip_reader_locate_file(&zip, path, NULL, 0);
	return entryIndex != -1;
}
bool ZIPFile::Is(const char *path) {
	SDL_RWops *rw = SDL_RWFromFile(path,"rb");
	if(!rw)
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
		"CRC check failed",
		"Unsupported cdir size",
		"Allocation failed",
		"File open failed",
		"File create failed",
		"File write failed",
		"File read failed",
		"File close failed",
		"File seek failed",
		"File stat failed",
		"Invalid parameter",
		"Invalid filename",
		"Buffer too small",
		"Internal error",
		"File not found",
		"Archive too large",
		"Validation failed",
		"Write callback failed"
	};
	return errors[err];
}
