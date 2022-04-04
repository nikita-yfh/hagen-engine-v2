#pragma once
#include <wx/propgrid/propgrid.h>
#include "b2_fixture.h"

WX_PG_DECLARE_VARIANT_DATA(b2Filter)

class FilterProperty : public wxPGProperty{
public:
	FilterProperty(const wxString& label = wxPG_LABEL,
				const wxString& name = wxPG_LABEL,
				const b2Filter &value = b2Filter());
	virtual void RefreshChildren() override;

	virtual wxVariant ChildChanged(wxVariant &thisValue,
								int childIndex,
								wxVariant &childValue) const override;
private:
	wxDECLARE_DYNAMIC_CLASS(FilterProperty);
};

