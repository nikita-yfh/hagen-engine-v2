#pragma once
#include "String.hpp"
#include "SDL.h"
#include "miniz.h"

class Storage {
public:
	Storage(const char *_path) : path(_path) {}

	virtual int Open() = 0;
	virtual int Close() = 0;

	virtual SDL_RWops *OpenFile(const char *file) = 0;
	virtual bool ExistFile(const char *file) = 0;

	Storage *next;

	inline const String &GetPath() const {
		return path;
	}
protected:
	String path;
};

class Directory : public Storage {
public:
	Directory(const char *path) : Storage(path) {}

	virtual int Open() override;
	virtual int Close() override;

	virtual SDL_RWops *OpenFile(const char *file) override;
	virtual bool ExistFile(const char *file) override;

	static bool Is(const char *path);
private:
	SDL_RWops *OpenFile(const char *file, const char *mode);
};

class ZIPFile : public Storage {
public:
	ZIPFile(const char *path)
		: Storage(path), data(nullptr), dataSize(0) {}

	virtual int Open() override;
	virtual int Close() override;

	virtual SDL_RWops *OpenFile(const char *path) override;
	virtual bool ExistFile(const char *path) override;

	static bool Is(const char *path);
private:
	static const char *ErrorToString(mz_zip_error);

	void *data;
	size_t dataSize;

	mz_zip_archive zip;
};
