#pragma once
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/propgrid/propgrid.h>
#include "Level.hpp"
#include "Camera.hpp"
#include "GLCanvas.hpp"
#include "ImagePanel.hpp"

enum{
	ID_PROPERTY_GRID=1,
	ID_IMAGE_ADD,
	ID_IMAGE_REMOVE,
	ID_IMAGE_PREVIEW,
	ID_IMAGE_LIST,
	ID_TEXTURE_RELOAD,
	ID_IMAGE_EDIT,
	ID_ADD_BODY,
	ID_ADD_BODY_STATIC,
	ID_ADD_BODY_KINEMATIC,
	ID_ADD_BODY_DYNAMIC,
	ID_ADD_JOINT,
	ID_ADD_JOINT_REVOLUTE,
	ID_ADD_JOINT_PRISMATIC,
	ID_ADD_JOINT_DISTANCE,
	ID_ADD_JOINT_PULLEY,
	ID_ADD_JOINT_GEAR,
	ID_ADD_JOINT_WHEEL,
	ID_ADD_JOINT_WELD,
	ID_ADD_JOINT_FRICTION,
	ID_ADD_JOINT_ROPE,
	ID_ADD_JOINT_MOTOR,
	ID_ADD_FIXTURE,
	ID_ADD_FIXTURE_CIRCLE,
	ID_ADD_FIXTURE_EDGE,
	ID_ADD_FIXTURE_POLYGON,
	ID_ADD_FIXTURE_CHAIN
};

class EditorFrame : public wxFrame {
public:
	EditorFrame(const Directory &directory);

	template<b2BodyType type>
	void AddBody(wxCommandEvent&){
		level.AddObject(new Body(type));
		canvas->Refresh();
	}
	template<class T>
	void AddFixture(wxCommandEvent&){
		level.AddFixture(new T());
		canvas->Refresh();
	}
	template<class T>
	void AddJoint(wxCommandEvent&){
		level.AddJoint(new T());
		canvas->Refresh();
	}
	template<void(Level::*method)()>
	void Execute(wxCommandEvent&){
		(level.*method)();
		canvas->Refresh();
	}

	void OneBodySelected(wxUpdateUIEvent&);
	void TwoBodiesSelected(wxUpdateUIEvent&);
	void OneNotDynamicBodySelected(wxUpdateUIEvent&);
	void OneSelected(wxUpdateUIEvent&);

private:
	void OnMouseMotion(wxMouseEvent&);
	void OnLevelUpdate(wxCommandEvent&);
	void OnPropertyGridUpdate(wxPropertyGridEvent&);
	void DeleteSelected(wxCommandEvent&);
	void SetImageEditMode(wxCommandEvent&);
	
	void SaveFile(wxCommandEvent&);
	void SaveFileAs(wxCommandEvent&);
	void OpenFile(wxCommandEvent&);

	void OnExit(wxCloseEvent&);

	void DisplayJSONError(const char *text, const rapidjson::Document &document) const;

	wxString lastFile;

	Level level;
	GLCanvas *canvas;
	ImagePanel *imagePanel;
	wxPropertyGrid *properties;
	Object *previous;

	wxDECLARE_EVENT_TABLE();
};
