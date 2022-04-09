#include "GUIConsole.hpp"

struct HistoryEntry{
	String str;
	HistoryEntry *next;
	HistoryEntry *prev;
};

GUIConsole::GUIConsole(lua_State *_L) : L(_L){
	*inputBuf = '\0';
	history = nullptr;
	historyPos = nullptr;
}
GUIConsole::~GUIConsole(){
	while(history){
		HistoryEntry *next = history->next;
		delete history;
		history = next;
	}
}

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
	if (scrollToBottom && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() || autoScroll))
		ImGui::SetScrollHereY(1.0f);
	scrollToBottom = false;
	ImGui::EndChild();
	ImGui::Separator();
    bool reclaimFocus = false;
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue |
		ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText(locale["console.input"], inputBuf, sizeof(inputBuf),
			flags, &GUIConsole::StaticInputCallback, this)) {
		if(*inputBuf){
			AddHistoryEntry(inputBuf);
			ExecCommand(inputBuf);
			*inputBuf = '\0';
			scrollToBottom = true;
		}
		reclaimFocus = true;
	}
	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
        ImGui::SetKeyboardFocusHere(-1);
	ImGui::End();
	return shown;
}
int GUIConsole::StaticInputCallback(ImGuiInputTextCallbackData *data){
	GUIConsole *console = (GUIConsole*)data->UserData;
	return console->InputCallback(data);
}
int GUIConsole::InputCallback(ImGuiInputTextCallbackData *data){
	switch (data->EventFlag) {
	case ImGuiInputTextFlags_CallbackCompletion:
		break;
	case ImGuiInputTextFlags_CallbackHistory:
		data->DeleteChars(0, data->BufTextLen);

		if(data->EventKey == ImGuiKey_UpArrow)
			HistoryUp();
		else
			HistoryDown();

		if(historyPos)
			data->InsertChars(data->CursorPos, historyPos->str);
		break;
	}
	return 0;
}
void GUIConsole::HistoryUp(){
	if(historyPos){
		if(historyPos->next)
			historyPos = historyPos->next;
	}else
		historyPos = history;
}
void GUIConsole::HistoryDown(){
	if(historyPos)
		historyPos = historyPos->prev;
}

void GUIConsole::ExecCommand(const char *cmd){
	Log(LEVEL_DEBUG, cmd);
}
void GUIConsole::AddHistoryEntry(const char *cmd){
	HistoryEntry *entry = new HistoryEntry;
	entry->str = cmd;
	entry->next = history;
	entry->prev = nullptr;
	if(history)
		history->prev = entry;
	history = entry;
	historyPos = nullptr;
}
