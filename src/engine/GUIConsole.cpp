#include "GUIConsole.hpp"

bool GUIConsole::Render(const Locale &locale){
	bool shown = true;
	ImGui::Begin("Console", &shown);
	ImGui::Text("Text");
	ImGui::End();
	return shown;
}
