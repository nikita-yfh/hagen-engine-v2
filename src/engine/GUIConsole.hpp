#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "ResourceManager.hpp"
#include "Interface.hpp"
#include "Locale.hpp"
#include "JSONUtils.hpp"

struct HistoryEntry;

class GUIConsole : public Window{
public:
	GUIConsole(ResourceManager *resManager, lua_State *L);
	~GUIConsole();

	void Render(const Locale &loc);
private:
	static int StaticInputCallback(ImGuiInputTextCallbackData *data);
	int InputCallback(ImGuiInputTextCallbackData *data);
	void ExecCommand(const char *cmd);
	void AddHistoryEntry(const char *cmd);
	void HistoryUp();
	void HistoryDown();

    char inputBuf[256];
    ImGuiTextFilter filter;
    bool autoScroll;
    bool scrollToBottom;
	HistoryEntry *history;
	HistoryEntry *historyPos;

    lua_State *L;

	struct Config{
		Config();

		bool enabled;
		bool input;
		bool colors;
		bool completion;
		bool history;

		bool FromJSON(const rapidjson::Value &value);
	}config;
};
