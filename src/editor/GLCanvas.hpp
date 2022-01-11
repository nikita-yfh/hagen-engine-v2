#pragma once
#include "Camera.hpp"
#include "Level.hpp"
#include "Colors.hpp"
#include <wx/glcanvas.h>
#include <wx/settings.h>

wxDECLARE_EVENT(EVENT_UPDATE_LEVEL, wxCommandEvent);

class GLCanvas : public wxGLCanvas {
public:
	GLCanvas(wxWindow *parent, Level &level);

	b2Vec2 GetMousePos() const;
	b2Vec2 GetGridMousePos() const;
private:
	void OnRightDown(wxMouseEvent&);
	void OnLeftDown(wxMouseEvent&);
	void OnLeftUp(wxMouseEvent&);
	void OnKey(wxKeyEvent&);
	void OnMouseMotion(wxMouseEvent&);
	void OnMouseWheel(wxMouseEvent&);
	void Draw(wxPaintEvent&);

	b2Vec2 GetMousePos(wxMouseEvent&) const;

	void SetMouse(const b2Vec2 &pixels);

	void Update();

	Level &level;
	Colors colors;
	Camera camera;
	b2Vec2 mouse;
	bool create;

	wxDECLARE_EVENT_TABLE();
};

