#include "EditorMain.hpp"
#include "ToolBar.hpp"
#include "MenuBar.hpp"
#include "GLCanvas.hpp"
#include "Edge.hpp"
#include "Circle.hpp"
#include "Polygon.hpp"
#include "Chain.hpp"
#include "WeldJoint.hpp"
#include "RevoluteJoint.hpp"
#include "PrismaticJoint.hpp"
#include "DistanceJoint.hpp"
#include "PulleyJoint.hpp"
#include <wx/sizer.h>
#include <wx/splitter.h>

wxBEGIN_EVENT_TABLE(EditorFrame,wxFrame)
	EVT_MOTION(EditorFrame::OnMouseMotion)
	EVT_PG_CHANGED(ID_PROPERTY_GRID, 		EditorFrame::OnPropertyGridUpdate)
	EVT_COMMAND(wxID_ANY,EVENT_UPDATE_LEVEL,EditorFrame::OnLevelUpdate)
	EVT_MENU(ID_ADD_BODY_STATIC, 	 		EditorFrame::AddBody<b2_staticBody>)
	EVT_MENU(ID_ADD_BODY_KINEMATIC,  		EditorFrame::AddBody<b2_kinematicBody>)
	EVT_MENU(ID_ADD_BODY_DYNAMIC, 	 		EditorFrame::AddBody<b2_dynamicBody>)
	EVT_MENU(ID_ADD_FIXTURE_CIRCLE,  		EditorFrame::AddFixture<Circle>)
	EVT_MENU(ID_ADD_FIXTURE_EDGE, 	 		EditorFrame::AddFixture<Edge>)
	EVT_MENU(ID_ADD_FIXTURE_POLYGON,	 	EditorFrame::AddFixture<Polygon>)
	EVT_MENU(ID_ADD_FIXTURE_CHAIN,		 	EditorFrame::AddFixture<Chain>)
	EVT_MENU(ID_ADD_JOINT_WELD,			 	EditorFrame::AddJoint<WeldJoint>)
	EVT_MENU(ID_ADD_JOINT_REVOLUTE,		 	EditorFrame::AddJoint<RevoluteJoint>)
	EVT_MENU(ID_ADD_JOINT_REVOLUTE,		 	EditorFrame::AddJoint<RevoluteJoint>)
	EVT_MENU(ID_ADD_JOINT_PRISMATIC,	 	EditorFrame::AddJoint<PrismaticJoint>)
	EVT_MENU(ID_ADD_JOINT_DISTANCE,		 	EditorFrame::AddJoint<DistanceJoint>)
	EVT_MENU(ID_ADD_JOINT_PULLEY,		 	EditorFrame::AddJoint<PulleyJoint>)
	EVT_MENU(wxID_SELECTALL,		 		EditorFrame::Execute<&Level::SelectAll>)
	EVT_MENU(wxID_DELETE,			 		EditorFrame::DeleteSelected)
	EVT_MENU(wxID_NEW,			 			EditorFrame::Execute<&Level::Clear>)
	EVT_UPDATE_UI(wxID_COPY,		 		EditorFrame::OneSelected)
	EVT_UPDATE_UI(wxID_CUT,			 		EditorFrame::OneSelected)
	EVT_UPDATE_UI(wxID_DUPLICATE,	 		EditorFrame::OneSelected)
	EVT_UPDATE_UI(ID_ADD_FIXTURE,			EditorFrame::OneBodySelected)
	EVT_UPDATE_UI(ID_ADD_FIXTURE_EDGE,		EditorFrame::OneNotDynamicBodySelected)
	EVT_UPDATE_UI(ID_ADD_FIXTURE_CHAIN,		EditorFrame::OneNotDynamicBodySelected)
	EVT_UPDATE_UI(ID_ADD_JOINT,				EditorFrame::TwoBodiesSelected)
wxEND_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString &gameDir)
		:wxFrame(nullptr,wxID_ANY,"Hagen Editor",wxDefaultPosition, {800,600}) {
	ToolBar *toolbar=new ToolBar(this);
	SetToolBar(toolbar);
	MenuBar *menubar=new MenuBar();
	SetMenuBar(menubar);

	wxBoxSizer *content=new wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow *vsplit=new wxSplitterWindow(this, wxID_ANY);
	{
		wxSplitterWindow *hsplit=new wxSplitterWindow(vsplit, wxID_ANY);
		{
			imagePanel=new ImagePanel(hsplit,gameDir,level);

			properties=new wxPropertyGrid(hsplit,ID_PROPERTY_GRID,wxDefaultPosition,
					wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER|wxPG_DEFAULT_STYLE);
			properties->SetSplitterLeft(true);

			hsplit->SplitHorizontally(imagePanel, properties);
		}
		canvas=new GLCanvas(vsplit,level);
		vsplit->SplitVertically(hsplit,canvas);
		vsplit->SetSashPosition(250);
	}
	content->Add(vsplit, 1, wxEXPAND);
	SetSizer(content);
	SetAutoLayout(true);
	CreateStatusBar(1);
}
void EditorFrame::OneBodySelected(wxUpdateUIEvent&e){
	e.Enable(level.GetSelectedBodyCount()==1);
}
void EditorFrame::TwoBodiesSelected(wxUpdateUIEvent&e){
	e.Enable(level.GetSelectedBodyCount()==2);
}
void EditorFrame::OneNotDynamicBodySelected(wxUpdateUIEvent&e){
	e.Enable(level.GetSelectedNotDynamicBodyCount()==1);
}
void EditorFrame::OneSelected(wxUpdateUIEvent&e){
	e.Enable(level.GetSelectedCount()>0);
}
void EditorFrame::OnMouseMotion(wxMouseEvent &e){
	SetStatusText(wxString::Format("%.03f, %.03f",
		canvas->GetMousePos().x,
		canvas->GetMousePos().y));
}
void EditorFrame::DeleteSelected(wxCommandEvent&e){
	level.DeleteSelected();
	OnLevelUpdate(e);
	canvas->Refresh();
}
void EditorFrame::OnLevelUpdate(wxCommandEvent&){
	Object *selected=level.GetFirstSelectedAll();
	if(selected != previous){
		properties->Clear();
		selected->UpdatePropertyGrid(properties, true);
		properties->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX, true);	
	}else if(selected && selected==previous)
		selected->UpdatePropertyGrid(properties,false);
	else
		properties->Clear();
	previous=selected;
}
void EditorFrame::OnPropertyGridUpdate(wxPropertyGridEvent &e){
	Object *selected=level.GetFirstSelectedAll();
	if(!selected)
		selected=&level;
	selected->OnPropertyGridChange(e.GetPropertyName(),e.GetPropertyValue());
	Refresh();
}
