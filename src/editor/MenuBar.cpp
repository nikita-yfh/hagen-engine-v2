#include "MenuBar.hpp"
#include "EditMenu.hpp"

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
}
