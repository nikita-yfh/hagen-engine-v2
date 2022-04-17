#pragma once
#include <wx/colour.h>
#include <wx/settings.h>

enum Color{
	COLOR_BLACK,
	COLOR_BGR,
	COLOR_GRID,
	COLOR_DARK,
	COLOR_INACTIVE,
	COLOR_ACTIVE,
	COLOR_BODY_STATIC,
	COLOR_BODY_KINEMATIC,
	COLOR_BODY_DYNAMIC,
	COLOR_JOINT,
	COLOR_JOINT_CONNECT,
	COLOR_COUNT
};

class Colors{
public:
	Colors();
	void Apply(Color, float alpha=1.0f) const;
	void ClearColor(Color) const;

	wxColor GetColor(Color) const;
private:
	static wxColor GetSystemColor(wxSystemColour);
	wxColor colors[COLOR_COUNT];
};
