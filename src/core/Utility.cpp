#include "Utility.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdarg>
#include <cerrno>
#include <cstdlib>
#include <cstring>

char *csprintf(const char *format, ...) {
	if(format==nullptr)
		return nullptr;
	va_list list;
	va_start(list,format);
	int numBytes=vsnprintf(nullptr,0,format,list)+1;
	char *buffer=(char*)malloc(numBytes);
	vsprintf(buffer,format,list);
	va_end(list);
	return buffer;
}

char *cstrdup(const char *src) {
	if(src==nullptr)
		return nullptr;
	char *str=(char*)malloc(strlen(src)+1);
	strcpy(str,src);
	return str;
}
static int do_mkdir(const char *path, unsigned int mode) {
	struct stat     st;
	int             status = 0;

	if (stat(path, &st) != 0) {
		if (mkdir(path, mode) != 0 && errno != EEXIST)
			status = -1;
	} else if (!S_ISDIR(st.st_mode)) {
		errno = ENOTDIR;
		status = -1;
	}

	return(status);
}
int mkpath(const char *path, unsigned int mode) {
	char           *pp;
	char           *sp;
	int             status;
	char           *copypath = cstrdup(path);

	status = 0;
	pp = copypath;
	while (status == 0 && (sp = strchr(pp, '/')) != 0) {
		if (sp != pp) {
			*sp = '\0';
			status = do_mkdir(copypath, mode);
			*sp = '/';
		}
		pp = sp + 1;
	}
	if (status == 0)
		status = do_mkdir(path, mode);
	free(copypath);
	return (status);
}
bool isdir(const char *path){
	struct stat info;
	return stat(path, &info) == 0 && info.st_mode & S_IFDIR;
}
