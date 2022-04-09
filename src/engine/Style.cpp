#include "Style.hpp"

bool Style::FromJSON(const rapidjson::Value &value){
	if(!jsonutils::CheckValue(value, "Colors"))
		return false;
	const rapidjson::Value &colors = value["Colors"];
	if(!jsonutils::CheckObject(colors))
		return false;
	for(int i = 0; i < ImGuiCol_COUNT; i++){
		const char *colorName = ImGui::GetStyleColorName(i);
		if(!jsonutils::GetMember(colors, colorName, Colors[i], false))
			return false;
	}
#define GETMEMBER(v) jsonutils::GetMember(value, #v, v, false)
	return
		GETMEMBER(Alpha) &&
		GETMEMBER(DisabledAlpha) &&
		GETMEMBER(WindowPadding) &&
		GETMEMBER(WindowRounding) &&
		GETMEMBER(WindowBorderSize) &&
		GETMEMBER(WindowMinSize) &&
		GETMEMBER(WindowTitleAlign) &&
		GETMEMBER(WindowMenuButtonPosition) &&
		GETMEMBER(ChildRounding) &&
		GETMEMBER(ChildBorderSize) &&
		GETMEMBER(PopupRounding) &&
		GETMEMBER(PopupBorderSize) &&
		GETMEMBER(FramePadding) &&
		GETMEMBER(FrameRounding) &&
		GETMEMBER(FrameBorderSize) &&
		GETMEMBER(ItemSpacing) &&
		GETMEMBER(ItemInnerSpacing) &&
		GETMEMBER(CellPadding) &&
		GETMEMBER(TouchExtraPadding) &&
		GETMEMBER(IndentSpacing) &&
		GETMEMBER(ColumnsMinSpacing) &&
		GETMEMBER(ScrollbarSize) &&
		GETMEMBER(ScrollbarRounding) &&
		GETMEMBER(GrabMinSize) &&
		GETMEMBER(GrabRounding) &&
		GETMEMBER(LogSliderDeadzone) &&
		GETMEMBER(TabRounding) &&
		GETMEMBER(TabBorderSize) &&
		GETMEMBER(TabMinWidthForCloseButton) &&
		GETMEMBER(ColorButtonPosition) &&
		GETMEMBER(ButtonTextAlign) &&
		GETMEMBER(SelectableTextAlign) &&
		GETMEMBER(DisplayWindowPadding) &&
		GETMEMBER(DisplaySafeAreaPadding) &&
		GETMEMBER(MouseCursorScale) &&
		GETMEMBER(AntiAliasedLines) &&
		GETMEMBER(AntiAliasedLinesUseTex) &&
		GETMEMBER(AntiAliasedFill) &&
		GETMEMBER(CurveTessellationTol) &&
		GETMEMBER(CircleTessellationMaxError);
#undef GETMEMBER
}
void Style::Apply() const {
	ImGui::GetStyle() = *this;
}

