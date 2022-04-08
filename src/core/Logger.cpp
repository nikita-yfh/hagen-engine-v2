#include "Logger.hpp"

#include <cstdio>
#ifdef ANDROID
#include <android/log.h>
#endif
#include "SDL.h" //for time

#ifdef __unix__
static const char *timeFormat = "[\033[0;36m%0.3f\033[0m]";
static const char *levels[LEVEL_COUNT] = {
	"[D]",
	"\033[0;32m[I]\033[0m",
	"\033[0;33m[W]\033[0m",
	"\033[0;31m[E]\033[0m",
	"\033[0;31m[F]\033[0m"
};
#else
static const char *timeFormat = "[%0.3f]";
static const char *levels[LEVEL_COUNT] = {
	"[D]",
	"[I]",
	"[W]",
	"[E]",
	"[F]"
};
#endif
#ifdef ANDROID
static const int androidLevels[LEVEL_COUNT] = {
	ANDROID_LEVEL_DEBUG,
	ANDROID_LEVEL_INFO,
	ANDROID_LEVEL_WARN,
	ANDROID_LEVEL_ERROR,
	ANDROID_LEVEL_FATAL
};
#endif

void Logger::Log(int level, const char *format, va_list args){
	if(level < 0)
		level = 0;
	if(level > LEVEL_FATAL)
		level = LEVEL_FATAL;

	FILE *file = stdout;
	if(level >= LEVEL_WARNING)
		file = stderr;

	char message[1024];
	vsnprintf(message, 1024, format, args);

	int time = SDL_GetTicks();
	char timeStr[24];
	sprintf(timeStr, timeFormat, time/1000.0f);

	fprintf(file, "%s %s %s\n", timeStr, levels[level], message);

#ifdef ANDROID
	__android_log_print(androidLevels[level], "HAGEN", "%s\n", message);
#endif
	LogEntry *entry = new LogEntry;
	entry->time = time;
	entry->level = level;
	entry->message = message;
	AddEntry(entry);
}
void Logger::Log(int level, const char *format, ...){
	va_list args;
	va_start(args, format);
	Log(level, format, args);
	va_end(args);
}
void Logger::AddEntry(LogEntry *entry){
	entry->next = nullptr;
	if(!items){
		items = entry;
	}else{
		LogEntry *end = items;
		while(end->next)
			end = end->next;
		end->next = entry;
	}
}

Logger::Logger()
	:items(nullptr) {}

Logger::~Logger(){
	Clear();
}

void Logger::Clear(){
	while(items){
		LogEntry *next = items->next;
		delete items;
		items = next;
	}
}

Logger &Logger::Instance(){
	static Logger instance;
	return instance;
}

ImVec4 LogEntry::GetColor() const{
	switch(level){
	case LEVEL_DEBUG:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	case LEVEL_INFO:
		return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	case LEVEL_WARNING:
		return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	case LEVEL_ERROR:
		return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	case LEVEL_FATAL:
		return ImVec4(0.7f, 0.0f, 0.0f, 1.0f);
	}
	return ImVec4();
}

void Log(int level, const char *format, ...){
	va_list args;
	va_start(args, format);
	Logger::Instance().Log(level, format, args);
	va_end(args);
}

