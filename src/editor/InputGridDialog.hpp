#pragma once
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

class InputGridDialog : public wxDialog {
public:
	InputGridDialog(wxWindow *parent, float grid);
	float GetGrid() const;

	static float InputGrid(wxWindow *parent, float grid);
private:
	wxSpinCtrlDouble *input;
};

