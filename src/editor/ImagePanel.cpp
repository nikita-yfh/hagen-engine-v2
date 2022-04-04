#include "ImagePanel.hpp"
#include "EditorMain.hpp"
#include "EditorApp.hpp"
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/dnd.h>

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_BUTTON(ID_IMAGE_ADD,		ImagePanel::OnPressButtonAdd)
	EVT_BUTTON(ID_IMAGE_REMOVE,		ImagePanel::OnPressButtonRemove)
	EVT_UPDATE_UI(ID_IMAGE_REMOVE,	ImagePanel::SelectedImage)
	EVT_LISTBOX(ID_IMAGE_LIST,		ImagePanel::OnSelectImage)
wxEND_EVENT_TABLE()

class ImagePanelDropTarget : public wxFileDropTarget{
public:
	ImagePanelDropTarget(ImagePanel *_panel)
		:panel(_panel) {}
	virtual bool OnDropFiles(wxCoord, wxCoord, const wxArrayString &files) override {
		for(const wxString &file : files)
			panel->AppendImage(file);
		return true;
	}
	ImagePanel *panel;
};
ImagePanel::ImagePanel(wxWindow *parent, Level &_level)
		:wxPanel(parent, wxID_ANY), level(_level), gameDir(_level.GetGameDir()) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);
		{
			buttons->Add(new wxButton(this,ID_IMAGE_ADD,	"+"), 1, wxALL, 2);
			buttons->Add(new wxButton(this,ID_IMAGE_REMOVE,	"-"), 1, wxALL, 2);
		}
		sizer->Add(buttons, 0, wxEXPAND);
	}
	{
		list = new wxListBox(this, ID_IMAGE_LIST); 
		sizer->Add(list, 1, wxEXPAND);
	}
	{
		preview = new ImagePreview(this, ID_IMAGE_PREVIEW);
		sizer->Add(preview, 0, wxEXPAND);
	}
	SetSizerAndFit(sizer);
	SetDropTarget(new ImagePanelDropTarget(this));
}

void ImagePanel::AppendImage(const wxString &fullPath){
	wxString lpath = gameDir.ConvertToLocalPath(fullPath);
	if(lpath != wxEmptyString){
		list->Append(lpath);
		level.AddTexture(lpath);
	}
}
void ImagePanel::RefreshList(){
	list->Clear();
	for(const Texture *tex = level.GetTextures(); tex; tex = tex->next)
		list->Append(tex->GetName());
}

void ImagePanel::OnPressButtonAdd(wxCommandEvent&){
	wxFileDialog dialog(this, wxFileSelectorPromptStr, gameDir.GetPath(), wxEmptyString, 
			"Images(*.png;*.jpg;*.jpeg)|*.png;*.jpg;*.jpeg");

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	AppendImage(dialog.GetPath());
}
void ImagePanel::OnSelectImage(wxCommandEvent &event){
	wxString path = gameDir.ConvertToAbsolutePath(list->GetString(event.GetInt()));
	preview->SetImage(path);
	Layout();
}

void ImagePanel::OnPressButtonRemove(wxCommandEvent&){
	int selection = list->GetSelection();
	wxString str = list->GetString(selection);
	list->Delete(selection);
	preview->DeleteImage();
	level.DeleteTexture(str);
	Layout();
}
void ImagePanel::SelectedImage(wxUpdateUIEvent &e){
	e.Enable(list->GetSelection() != wxNOT_FOUND);
}
