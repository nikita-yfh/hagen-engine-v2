#pragma once
#include <wx/app.h>

class EditorApp : public wxApp {
public:
	virtual bool OnInit() override;
private:
	wxLocale m_Locale;
};

DECLARE_APP(EditorApp);
