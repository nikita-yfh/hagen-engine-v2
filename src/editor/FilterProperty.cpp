#include "FilterProperty.hpp"

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(b2Filter)

wxIMPLEMENT_DYNAMIC_CLASS(FilterProperty, wxPGProperty);

FilterProperty::FilterProperty(const wxString &label, const wxString &name,
				const b2Filter &value) : wxPGProperty(label,name){
	m_value=WXVARIANT(value);

	AddPrivateChild(new wxIntProperty("category", wxPG_LABEL, value.categoryBits));
	AddPrivateChild(new wxIntProperty("mask", wxPG_LABEL, value.maskBits));
	AddPrivateChild(new wxIntProperty("group", wxPG_LABEL, value.groupIndex));
}

wxVariant FilterProperty::ChildChanged(wxVariant &thisValue,
									int childIndex,
									wxVariant &childValue) const{
	b2Filter vec;
	vec << m_value;

	if(childIndex==0)
		vec.categoryBits=childValue.GetLong();
	else if(childIndex==1)
		vec.maskBits=childValue.GetLong();
	else if(childIndex==2)
		vec.groupIndex=childValue.GetLong();
	
	return WXVARIANT(vec);
}
void FilterProperty::RefreshChildren(){
	if(!GetChildCount())
		return;
	b2Filter vec;
	vec << m_value;
	Item(0)->SetValue(vec.categoryBits);
	Item(1)->SetValue(vec.maskBits);
	Item(2)->SetValue(vec.groupIndex);
}
