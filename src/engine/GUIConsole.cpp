#include "GUIConsole.hpp"

bool GUIConsole::Render(const Locale &locale){
	bool shown = true;
	ImGui::Begin(locale["console.title"], &shown);

	const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y +
		ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("scrolling", ImVec2(0, -footerHeightToReserve),
		false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::Selectable(locale["console.clear"]))
			Logger::Instance().Clear();
		ImGui::EndPopup();
	}
	for (LogEntry *entry = Logger::Instance().items; entry; entry = entry->next) {
		if (!filter.PassFilter(entry->message))
			continue;
		ImGui::PushStyleColor(ImGuiCol_Text, entry->GetColor());
		ImGui::TextWrapped(entry->message.c_str());
		ImGui::PopStyleColor();
	}
	if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY() && scrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::End();
	return shown;
}
