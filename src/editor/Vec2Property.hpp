#pragma once
#include <wx/propgrid/propgrid.h>
#include "b2_math.h"

WX_PG_DECLARE_VARIANT_DATA(b2Vec2)

class Vec2Property : public wxPGProperty{
public:
	Vec2Property(const wxString& label = wxPG_LABEL,
				const wxString& name = wxPG_LABEL,
				const b2Vec2 &value = b2Vec2_zero);
	virtual void RefreshChildren() override;

	virtual wxVariant ChildChanged(wxVariant &thisValue,
								int childIndex,
								wxVariant &childValue) const override;
private:
	wxDECLARE_DYNAMIC_CLASS(Vec2Property);
};

