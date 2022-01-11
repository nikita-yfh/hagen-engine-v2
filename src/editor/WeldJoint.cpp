#include "WeldJoint.hpp"

WeldJoint::WeldJoint()
	:stiffness(0.0f), damping(0.0f) {}

void WeldJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n){
		pg->Append(new wxFloatProperty("Stiffness", wxPG_LABEL, stiffness));
		pg->Append(new wxFloatProperty("Damping", wxPG_LABEL, damping));
	}
	PointJoint::UpdatePropertyGrid(pg,n);
}
void WeldJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "Stiffness")
		stiffness = value.GetDouble();
	else if(name == "Damping")
		damping = value.GetDouble();
	PointJoint::OnPropertyGridChange(name, value);
}
