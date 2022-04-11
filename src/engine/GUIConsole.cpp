#include "GUIConsole.hpp"

struct HistoryEntry{
	String str;
	HistoryEntry *next;
	HistoryEntry *prev;
};

GUIConsole::GUIConsole(ResourceManager &resManager, lua_State *_L) : L(_L){
	*inputBuf = '\0';
	history = nullptr;
	historyPos = nullptr;
	resManager.LoadJSON("ui/console.json", config);
}
GUIConsole::~GUIConsole(){
	while(history){
		HistoryEntry *next = history->next;
		delete history;
		history = next;
	}
}

void GUIConsole::Render(const Locale &locale){
	if(!config.enabled){
		Hide();
		return;
	}
	ImGui::Begin(locale["console.title"], &shown);

	float reserve = 0.0f;
	if(config.input)
		reserve = -ImGui::GetStyle().ItemSpacing.y - ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("scrolling", ImVec2(0.0f, reserve),
		false, ImGuiWindowFlags_HorizontalScrollbar);

	if(ImGui::BeginPopupContextWindow()) {
		if(ImGui::Selectable(locale["console.clear"]))
			Logger::Instance().Clear();
		ImGui::EndPopup();
	}
	for(LogEntry *entry = Logger::Instance().items; entry; entry = entry->next) {
		if(!filter.PassFilter(entry->message))
			continue;
		if(config.colors){
			ImGui::PushStyleColor(ImGuiCol_Text, entry->GetColor());
			ImGui::TextWrapped("%s", entry->message.c_str());
			ImGui::PopStyleColor();
		}else
			ImGui::TextWrapped("%s", entry->message.c_str());

	}
	if(scrollToBottom &&(ImGui::GetScrollY() >= ImGui::GetScrollMaxY() || autoScroll))
		ImGui::SetScrollHereY(1.0f);
	scrollToBottom = false;
	ImGui::EndChild();
	if(config.input){
		ImGui::Separator();
		bool reclaimFocus = false;
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
		if(config.history)
			flags |= ImGuiInputTextFlags_CallbackHistory;
		if(config.completion)
			flags |= ImGuiInputTextFlags_CallbackCompletion;
		if(ImGui::InputText(locale["console.input"], inputBuf, sizeof(inputBuf),
				flags, &GUIConsole::StaticInputCallback, this)) {
			if(*inputBuf){
				if(config.history)
					AddHistoryEntry(inputBuf);
				ExecCommand(inputBuf);
				*inputBuf = '\0';
				scrollToBottom = true;
			}
			reclaimFocus = true;
		}
		ImGui::SetItemDefaultFocus();
		if(reclaimFocus)
			ImGui::SetKeyboardFocusHere(-1);
	}
	ImGui::End();
}
int GUIConsole::StaticInputCallback(ImGuiInputTextCallbackData *data){
	GUIConsole *console =(GUIConsole*)data->UserData;
	return console->InputCallback(data);
}
int GUIConsole::InputCallback(ImGuiInputTextCallbackData *data){
	switch(data->EventFlag) {
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
	if(luaL_dostring(L, cmd))
		Log(LEVEL_ERROR, lua_tostring(L, -1));
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

GUIConsole::Config::Config() :
	enabled(true), input(true), colors(true),
	history(true), completion(true) {}

bool GUIConsole::Config::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::CheckObject(value) &&
		jsonutils::GetMember(value, "enabled", enabled, false) &&
		jsonutils::GetMember(value, "input", input, false) &&
		jsonutils::GetMember(value, "colors", colors, false) &&
		jsonutils::GetMember(value, "history", history, false) &&
		jsonutils::GetMember(value, "completion", completion, false);
}
