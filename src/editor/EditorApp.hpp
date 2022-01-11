#pragma once
#include <wx/app.h>

class EditorApp : public wxApp {
public:
	virtual bool OnInit();
private:
	wxLocale m_Locale;
};

DECLARE_APP(EditorApp);
