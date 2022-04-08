#include "GUIConsole.hpp"

GUIConsole::GUIConsole(const Locale &_loc, lua_State *_L)
					:loc(_loc), L(_L) {}

bool GUIConsole::Render(){
	bool shown = true;
	ImGui::Begin("Console", &shown);
	ImGui::Text("Text");
	ImGui::End();
	return shown;
}
