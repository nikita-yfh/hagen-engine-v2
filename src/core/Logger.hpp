#pragma once
#include "imgui.h"
#include "String.hpp"

enum {
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARNING,
	LEVEL_ERROR,
	LEVEL_FATAL,

	LEVEL_COUNT
};
struct LogEntry{
	int time;
	int level;
	String message;

	ImVec4 GetColor() const;

	LogEntry *next;
};
class Logger{
public:
	~Logger();

	void Log(int level, const char *format, ...);
	void Log(int level, const char *format, va_list args);
	void Clear();

	static Logger &Instance();

	LogEntry *items;
private:
	void AddEntry(LogEntry *entry);
	static const int maxSize = 100;
	Logger();
};

void Log(int level, const char *format, ...);
