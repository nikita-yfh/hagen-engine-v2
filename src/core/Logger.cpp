#include "Logger.hpp"

#include <cstdio>
#ifdef ANDROID
#include <android/log.h>
#endif
#include "SDL.h" //for time

LogEntry::LogEntry(int _time,int _level,const char *_message,LogEntry *_next):
		time(_time),level(_level),next(_next){
	message=(char*)malloc(strlen(_message)+1);
	strcpy(message,_message);
}
LogEntry::~LogEntry(){
	free(message);
}

#ifdef __unix__
static const char *time_format="[\033[0;36m%0.3f\033[0m]";
static const char *levels[LEVEL_COUNT]={
	"[D]",
	"\033[0;32m[I]\033[0m",
	"\033[0;33m[W]\033[0m",
	"\033[0;31m[E]\033[0m",
	"\033[0;31m[F]\033[0m"
};
#else
static const char *time_format="[%0.3f]";
static const char *levels[LEVEL_COUNT]={
	"[D]",
	"[I]",
	"[W]",
	"[E]",
	"[F]"
};
#endif
#ifdef ANDROID
static const int androidLevels[LEVEL_COUNT]={
	ANDROID_LEVEL_DEBUG,
	ANDROID_LEVEL_INFO,
	ANDROID_LEVEL_WARN,
	ANDROID_LEVEL_ERROR,
	ANDROID_LEVEL_FATAL
};
#endif

void Logger::Log(int level,const char *format,va_list args){
	if(level<0)level=0;
	if(level>LEVEL_FATAL)level=LEVEL_FATAL;

	FILE *file;
	if(level>=LEVEL_WARNING)
		file=stderr;
	else
		file=stdout;

	char message[1024];
	vsprintf(message,format,args);

	int time_ms=SDL_GetTicks();

	char time[24];
	sprintf(time,time_format,time_ms/1000.0f);

	fprintf(file,"%s %s %s\n",time,levels[level],message);

#ifdef ANDROID
	__android_log_print(androidLevels[level], "HAGEN", "%s\n",message);
#endif
	LogEntry *entry=new LogEntry(time_ms,level,message,items);
	items=entry;
}
void Logger::Log(int level,const char *format, ...){
	va_list args;
	va_start(args,format);
	Log(level,format,args);
	va_end(args);
}

Logger::Logger()
	:items(nullptr) {}
Logger::~Logger(){
	Clear();
}

void Logger::Clear(){
	LogEntry *entry=items;
	while(entry){
		LogEntry *temp=entry->next;
		delete entry;
		entry=temp;
	}
}

Logger &Logger::Instance(){
	static Logger instance;
	return instance;
}

ImVec4 LogEntry::GetColor() const{
	ImVec4 color;
	switch(level){
	case LEVEL_DEBUG:
		color=ImVec4(1.0f,1.0f,1.0f,1.0f);
		break;
	case LEVEL_INFO:
		color=ImVec4(0.0f,1.0f,0.0f,1.0f);
		break;
	case LEVEL_WARNING:
		color=ImVec4(1.0f,1.0f,0.0f,1.0f);
		break;
	case LEVEL_ERROR:
		color=ImVec4(1.0f,0.0f,0.0f,1.0f);
		break;
	case LEVEL_FATAL:
		color=ImVec4(0.7f,0.0f,0.0f,1.0f);
		break;
	}
	return color;
}

void Log(int level,const char *format, ...){
	va_list args;
	va_start(args,format);
	Logger::Instance().Log(level,format,args);
	va_end(args);
}

