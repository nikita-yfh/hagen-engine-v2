#pragma once
#include "Level.hpp"
#include "ImagePreview.hpp"
#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/string.h>

class ImagePanel : public wxPanel{
public:
	ImagePanel(wxWindow *parent, const wxString &gameDir, Level &level);
private:
	void OnPressButtonAdd(wxCommandEvent&);
	void OnPressButtonRemove(wxCommandEvent&);
	void OnSelectImage(wxCommandEvent&);
	void SelectedImage(wxUpdateUIEvent&);

	Level &level;
	wxString gameDir;

	wxListBox *list;
	ImagePreview *preview;

	wxDECLARE_EVENT_TABLE();
};
