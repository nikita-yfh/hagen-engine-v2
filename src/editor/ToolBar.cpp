#include "ToolBar.hpp"
#include <wx/artprov.h>

#define ICON(i) wxArtProvider::GetBitmap(i, wxART_TOOLBAR, wxDefaultSize)

ToolBar::ToolBar(wxWindow *parent)
		:wxToolBar(parent,wxID_ANY,wxDefaultPosition,
		wxDefaultSize,wxTB_HORIZONTAL|wxTB_FLAT){
	
	AddTool(wxID_NEW, _("New"),ICON(wxART_NEW));
	AddTool(wxID_OPEN, _("Open"),ICON(wxART_FILE_OPEN));
	AddTool(wxID_SAVE, _("Save"),ICON(wxART_FILE_SAVE));
	AddSeparator();
	AddTool(wxID_UNDO, _("Undo"),ICON(wxART_UNDO));
	AddTool(wxID_REDO, _("Redo"),ICON(wxART_REDO));
}
