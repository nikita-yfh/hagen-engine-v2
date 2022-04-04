#include "Vec2Property.hpp"

WX_PG_IMPLEMENT_VARIANT_DATA(b2Vec2)

wxIMPLEMENT_DYNAMIC_CLASS(Vec2Property, wxPGProperty);

Vec2Property::Vec2Property(const wxString &label, const wxString &name,
				const b2Vec2 &value) :wxPGProperty(label,name){
	m_value=WXVARIANT(value);

	AddPrivateChild(new wxFloatProperty("x", wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty("y", wxPG_LABEL, value.y));
}

wxVariant Vec2Property::ChildChanged(wxVariant &thisValue,
									int childIndex,
									wxVariant &childValue) const{
	b2Vec2 vec;
	vec << m_value;

	if(childIndex==0)
		vec.x=childValue.GetDouble();
	else if(childIndex==1)
		vec.y=childValue.GetDouble();
	
	return WXVARIANT(vec);
}
void Vec2Property::RefreshChildren(){
	if(!GetChildCount())
		return;
	b2Vec2 vec;
	vec << m_value;
	Item(0)->SetValue((float)vec.x);
	Item(1)->SetValue((float)vec.y);
}
