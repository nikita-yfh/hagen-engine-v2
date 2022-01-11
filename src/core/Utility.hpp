#pragma once

char *csprintf(const char *format, ...);
char *cstrdup(const char *src);
int mkpath(const char *path, unsigned int mode);
bool isdir(const char *path);
