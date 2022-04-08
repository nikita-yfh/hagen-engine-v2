#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Interface.hpp"
#include "Locale.hpp"

class GUIConsole : public Window{
public:
	GUIConsole(lua_State *_L) : L(_L) {}

	bool Render(const Locale &loc);
private:
    char inputBuf[256];
    int historyPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter filter;
    bool autoScroll;
    bool scrollToBottom;

    lua_State *L;
};
