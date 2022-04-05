#include "InputGridDialog.hpp"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>

InputGridDialog::InputGridDialog(wxWindow* parent, float grid)
		:wxDialog(parent, wxID_ANY, "Set grid size") {
	wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
	input=new wxSpinCtrlDouble(this,wxID_ANY, wxEmptyString, wxDefaultPosition,
		wxDefaultSize, wxSP_ARROW_KEYS, 0.01, 100.0, grid, 0.125);
	input->SetDigits(3);
	box->Add(input,1,wxEXPAND|wxALL,5);
	box->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL);
	input->SetFocus();
	SetSizerAndFit(box);
}

float InputGridDialog::GetGrid() const {
	return input->GetValue();
}

float InputGridDialog::InputGrid(wxWindow *parent, float grid) {
	InputGridDialog dialog(parent, grid);
	if(dialog.ShowModal() == wxID_OK)
		return dialog.GetGrid();
	return grid;
}
