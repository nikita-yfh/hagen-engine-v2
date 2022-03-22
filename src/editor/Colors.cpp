#include "Colors.hpp"
#include <wx/glcanvas.h>

Colors::Colors(){
	colors[COLOR_BGR]			= GetSystemColor(wxSYS_COLOUR_WINDOW);
	colors[COLOR_GRID]			= GetSystemColor(wxSYS_COLOUR_GRAYTEXT);
	colors[COLOR_DARK]			= GetSystemColor(wxSYS_COLOUR_BTNTEXT);
	colors[COLOR_INACTIVE]		= GetSystemColor(wxSYS_COLOUR_GRAYTEXT);
	colors[COLOR_ACTIVE]		= GetSystemColor(wxSYS_COLOUR_MENUHILIGHT);
	colors[COLOR_BODY_STATIC]	= wxColor(128,230,128,255);
	colors[COLOR_BODY_KINEMATIC]= wxColor(128,128,230,255);
	colors[COLOR_BODY_DYNAMIC]	= wxColor(230,178,178,255);
	colors[COLOR_JOINT]			= colors[COLOR_ACTIVE];
	colors[COLOR_JOINT_CONNECT] = colors[COLOR_INACTIVE];
}
wxColor Colors::GetSystemColor(wxSystemColour c){
	return wxSystemSettings::GetColour(c);
}
wxColor Colors::GetColor(Color c) const{
	return colors[c];
}
void Colors::Apply(Color c, float alpha) const{
	wxColor color=GetColor(c);
	glColor4ub(
		color.Red(),
		color.Green(),
		color.Blue(),
		color.Alpha()*alpha
	);
}
void Colors::ClearColor(Color c) const{
	wxColor color=GetColor(c);
	glClearColor(
		color.Red()/255.0f,
		color.Green()/255.0f,
		color.Blue()/255.0f,
		color.Alpha()/255.0f
	);
}
