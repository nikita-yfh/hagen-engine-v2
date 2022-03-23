#include "MenuBar.hpp"
#include "EditMenu.hpp"
#include "EditorMain.hpp"

MenuBar::MenuBar() {
	{
		//File
		wxMenu* menu= new wxMenu();
		menu->Append(wxID_NEW,_("New\tCtrl+N"));
		menu->Append(wxID_OPEN,_("Open\tCtrl+O"));
		menu->Append(wxID_SAVE,_("Save\tCtrl+S"));
		menu->Append(wxID_SAVEAS,_("Save as\tCtrl+Shift+S"));
		menu->Append(wxID_EXIT,_("Exit\tCtrl+Q"));
		Append(menu, _("File"));
	}
	{
		//Edit
		EditMenu *menu=new EditMenu();
		Append(menu,_("Edit"));
	}
	{
		//File
		wxMenu* menu= new wxMenu();
		menu->Append(ID_TEXTURE_RELOAD,_("Reload textures\tCtrl+R"));
		menu->Append(ID_IMAGE_EDIT, _("Image edit mode\tCtrl-I"), wxEmptyString, wxITEM_CHECK);
		Append(menu, _("Level"));
	}
}
