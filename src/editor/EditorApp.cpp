#include "EditorApp.hpp"
#include "EditorMain.hpp"
#include "b2_math.h"
#include <wx/dirdlg.h>

IMPLEMENT_APP(EditorApp);

const b2Vec2 b2Vec2_zero(0.0f,0.0f);	

bool EditorApp::OnInit() {
	wxString gameDir;
	if(argc>=2)
		gameDir=argv[1];
	else{
		wxDirDialog dialog(nullptr, "Choose game directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		
		if (dialog.ShowModal() == wxID_CANCEL)
			return false;
		gameDir = dialog.GetPath();
	}
	EditorFrame* frame;
	frame=new EditorFrame(gameDir);
	frame->Show();
	SetTopWindow(frame);
	return true;
}
