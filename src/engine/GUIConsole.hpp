#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Interface.hpp"

class GUIConsole : Window{
public:
	GUIConsole(lua_State *L);
	~GUIConsole();

	void Render();
private:
    char                  inputBuf[256];
    int                   historyPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       filter;
    bool                  autoScroll;
    bool                  scrollToBottom;

    lua_State *L;
};
