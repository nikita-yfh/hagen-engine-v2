#pragma once
#include "Level.hpp"
#include "ImagePreview.hpp"
#include <wx/panel.h>
#include <wx/listbox.h>

class ImagePanel : public wxPanel {
public:
	ImagePanel(wxWindow *parent, Level &level);
	void AppendImage(const wxString&);
	void RefreshList();
private:
	void OnPressButtonAdd(wxCommandEvent&);
	void OnPressButtonRemove(wxCommandEvent&);
	void OnSelectImage(wxCommandEvent&);
	void SelectedImage(wxUpdateUIEvent&);

	Level &level;
	const Directory &gameDir;

	wxListBox *list;
	ImagePreview *preview;

	wxDECLARE_EVENT_TABLE();
};
