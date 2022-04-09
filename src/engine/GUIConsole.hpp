#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Interface.hpp"
#include "Locale.hpp"

struct HistoryEntry;

class GUIConsole : public Window{
public:
	GUIConsole(lua_State *L);
	~GUIConsole();

	bool Render(const Locale &loc);
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
};
