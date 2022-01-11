#include "GLCanvas.hpp"
#include "EditMenu.hpp"
#include <wx/utils.h>

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
GLCanvas::GLCanvas(wxWindow *parent, Level &_level)
		:wxGLCanvas(parent,wxID_ANY,attribList), level(_level), create(false) {
	SetFocus();
}
b2Vec2 GLCanvas::GetMousePos(wxMouseEvent &e) const{
	wxPoint global=e.GetPosition();
	wxSize size=GetSize();
	return b2Vec2(global.x-size.x/2,global.y-size.y/2);

}
b2Vec2 GLCanvas::GetMousePos() const{
	return mouse;
}
b2Vec2 GLCanvas::GetGridMousePos() const{
	return camera.ToGrid(mouse);
}
void GLCanvas::Update(){
	wxPostEvent(GetParent()->GetParent(), wxCommandEvent(EVENT_UPDATE_LEVEL));
	Refresh();
}
void GLCanvas::SetMouse(const b2Vec2 &pixels){
	mouse=camera.ConvertFromPixels(pixels);
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
	float n=(e.GetWheelRotation()>0)?1.05:0.95;
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
	if(e.LeftIsDown() || level.IsCreating()){
		if(level.UpdatePoints(Mouse(mouse,false,camera))){
			Update();
		}
	}
	wxPostEvent(GetParent(),e);
}
void GLCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	colors.ClearColor(COLOR_BGR);
	glClear(GL_COLOR_BUFFER_BIT);
	wxSize size=GetSize();
	wxSize halfsize=size/2;
	glViewport(0, 0,size.x,size.y);
	glLoadIdentity();
	glOrtho(-halfsize.x, halfsize.x, halfsize.y, -halfsize.y, 0.0f, 1.0f);

	camera.Apply(colors,halfsize);
	level.Draw(colors);
	if(camera.zoom>15)
		level.DrawPoints(colors);

	glFlush();
	SwapBuffers();
}
