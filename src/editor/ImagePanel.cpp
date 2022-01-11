#include "ImagePanel.hpp"
#include "EditorMain.hpp"
#include <wx/sizer.h>
#include <wx/filedlg.h>

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_BUTTON(ID_IMAGE_ADD,		ImagePanel::OnPressButtonAdd)
	EVT_BUTTON(ID_IMAGE_REMOVE,		ImagePanel::OnPressButtonRemove)
	EVT_UPDATE_UI(ID_IMAGE_REMOVE,	ImagePanel::SelectedImage)
	EVT_LISTBOX(ID_IMAGE_LIST,		ImagePanel::OnSelectImage)
wxEND_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent, const wxString &_gameDir, Level &_level)
		:wxPanel(parent, wxID_ANY), level(_level), gameDir(_gameDir) {
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
	wxInitAllImageHandlers();
}

void ImagePanel::OnPressButtonAdd(wxCommandEvent&){
	wxFileDialog dialog(this, wxFileSelectorPromptStr, gameDir, wxEmptyString, 
			"Images(*.png;*.jpg;*.jpeg)|*.png;*.jpg;*.jpeg");

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	wxString path=dialog.GetPath();
	path.Remove(0,gameDir.Length()+1); //convert to local path
	list->Append(path);
}
void ImagePanel::OnSelectImage(wxCommandEvent &event){
	wxString path = gameDir + "/" + list->GetString(event.GetInt());
	preview->SetImage(path);
	Layout();
}

void ImagePanel::OnPressButtonRemove(wxCommandEvent&){
	int selection = list->GetSelection();
	wxString str = list->GetString(selection);
	list->Delete(selection);
	preview->DeleteImage();
	Layout();
}
void ImagePanel::SelectedImage(wxUpdateUIEvent &e){
	e.Enable(list->GetSelection() != wxNOT_FOUND);
}
