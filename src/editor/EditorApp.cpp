#include "EditorApp.hpp"
#include "EditorMain.hpp"
#include "Directory.hpp"
#include "b2_math.h"
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include <wx/filename.h>

IMPLEMENT_APP(EditorApp);

const b2Vec2 b2Vec2_zero(0.0f,0.0f);	

bool EditorApp::OnInit() {
	Directory gameDir;
	if(argc>=2){
		if(!wxDirExists(argv[1])){
			wxMessageBox("Directory not found", "Error", wxICON_ERROR);
			return false;
		}
		wxFileName dir = wxFileName::DirName(argv[1]);
		dir.MakeAbsolute();
		gameDir = dir.GetPath();
	}else{
		wxDirDialog dialog(nullptr, "Choose game directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		
		if (dialog.ShowModal() == wxID_CANCEL)
			return false;
		gameDir = dialog.GetPath();
	}
	wxInitAllImageHandlers();
	EditorFrame* frame=new EditorFrame(gameDir);
	frame->Show();
	return true;
}
