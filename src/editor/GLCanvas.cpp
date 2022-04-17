#include "GLCanvas.hpp"
#include "InputGridDialog.hpp"
#include "EditMenu.hpp"
#include "Texture.hpp"
#include <wx/utils.h>
#include <wx/dnd.h>
#include <iostream>

wxBEGIN_EVENT_TABLE(GLCanvas,wxGLCanvas)
	EVT_MOTION(GLCanvas::OnMouseMotion)
	EVT_MOUSEWHEEL(GLCanvas::OnMouseWheel)
	EVT_RIGHT_DOWN(GLCanvas::OnRightDown)
	EVT_LEFT_DOWN(GLCanvas::OnLeftDown)
	EVT_LEFT_UP(GLCanvas::OnLeftUp)
	EVT_KEY_DOWN(GLCanvas::OnKey)
	EVT_KEY_UP(GLCanvas::OnKey)
	EVT_PAINT(GLCanvas::Draw)
wxEND_EVENT_TABLE()

wxDEFINE_EVENT(EVENT_UPDATE_LEVEL, wxCommandEvent);

static const int attribList[] = {
	WX_GL_RGBA,
	WX_GL_SAMPLE_BUFFERS, GL_FALSE,
	WX_GL_DEPTH_SIZE, 24, 0, 0
};
class CanvasDropTarget : public wxFileDropTarget{
public:
	CanvasDropTarget(GLCanvas *_canvas, Level &_level)
		:canvas(_canvas), level(_level) {}
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override{
		if(!Image::enabled)
			return false;
		level.AddImage(filenames[0]);
		return true;
	}
	GLCanvas *canvas;
	Level &level;
};

GLCanvas::GLCanvas(wxWindow *parent, Level &_level)
		:wxGLCanvas(parent,wxID_ANY,attribList), level(_level) {
	SetFocus();
	SetDropTarget(new CanvasDropTarget(this, level));
}
void GLCanvas::SetGridSize(){
	camera.SetGrid(InputGridDialog::InputGrid(this, camera.gridSize));
	Refresh();
}
void GLCanvas::SetPreview(){
	if(level.IsCreating())
		level.CancelCreating();
	preview = !preview;
	Refresh();
}
b2Vec2 GLCanvas::GetMousePos(wxMouseEvent &e) const{
	wxPoint global=e.GetPosition();
	wxSize size=GetSize();
	return b2Vec2(global.x-size.x/2, global.y-size.y/2);

}
b2Vec2 GLCanvas::GetMousePos() const{
	return mouse;
}
b2Vec2 GLCanvas::GetGridMousePos() const{
	return camera.ToGrid(mouse);
}
void GLCanvas::Update(){
	wxPostEvent(GetParent(), wxCommandEvent(EVENT_UPDATE_LEVEL));
	Refresh();
}
void GLCanvas::SetMouse(const b2Vec2 &pixels){
	mouse = camera.ConvertFromPixels(pixels);
}
void GLCanvas::OnRightDown(wxMouseEvent&e) {
	if(level.IsCreating()){
		level.CancelCreating();
		Update();
	}else{
		EditMenu *menu=new EditMenu();
		PopupMenu(menu,e.GetPosition());
	}
	e.Skip();
}
void GLCanvas::OnLeftDown(wxMouseEvent&e) {
	if(preview)
		return;
	SetFocus();
	if(!level.IsCreating() && !wxGetKeyState(WXK_SHIFT))
		level.UnselectAll();
	SetMouse(GetMousePos(e));
	if(level.UpdatePoints(Mouse(mouse,true,camera)))
		Refresh();
}
void GLCanvas::OnLeftUp(wxMouseEvent&e) {
	if(!level.IsCreating())
		level.UnselectAllPoints();
	Update();
}
void GLCanvas::OnKey(wxKeyEvent&e) {
	if(camera.UpdateGrid(wxGetKeyState(WXK_SHIFT),wxGetKeyState(WXK_CONTROL)))
		Refresh();
	e.Skip();
}
void GLCanvas::OnMouseWheel(wxMouseEvent &e){
	float n=(e.GetWheelRotation()>0)?1.05f:0.95f;
	b2Vec2 currentMouse=GetMousePos(e);
	camera.Zoom(n,mouse,currentMouse);
	SetMouse(currentMouse);
	Refresh();
}
void GLCanvas::OnMouseMotion(wxMouseEvent &e){
	if(e.MiddleIsDown()){
		b2Vec2 currentMouse=GetMousePos(e);
		camera.Drag(mouse,currentMouse);
		SetMouse(currentMouse);
		Refresh();
	}else
		SetMouse(GetMousePos(e));
	if(e.LeftIsDown() || level.IsCreating())
		if(level.UpdatePoints(Mouse(mouse,false,camera)))
			Update();
}
void GLCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	colors.ClearColor(preview?COLOR_BLACK:COLOR_BGR);
	glClear(GL_COLOR_BUFFER_BIT);
	wxSize size=GetSize();
	wxSize halfsize=size/2;
	glViewport(0, 0,size.x,size.y);
	glLoadIdentity();
	glOrtho(-halfsize.x, halfsize.x, halfsize.y, -halfsize.y, 0.0f, 1.0f);

	if(!preview)
		camera.DrawGrid(colors,halfsize);
	camera.Apply();
	if(preview)
		level.DrawPreview();
	else{
		level.Draw(colors);
		if(camera.zoom>15)
			level.DrawPoints(colors);
	}
	glFlush();
	SwapBuffers();
}
