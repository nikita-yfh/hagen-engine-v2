#include "WeldJoint.hpp"

WeldJoint::WeldJoint()
	:stiffness(0.0f), damping(0.0f) {}

void WeldJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n){
		pg->Append(new wxFloatProperty("stiffness", wxPG_LABEL, stiffness));
		pg->Append(new wxFloatProperty("damping", wxPG_LABEL, damping));
	}
	PointJoint::UpdatePropertyGrid(pg,n);
}
void WeldJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "stiffness")
		stiffness = value.GetDouble();
	else if(name == "damping")
		damping = value.GetDouble();
	PointJoint::OnPropertyGridChange(name, value);
}
void WeldJoint::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "weld", allocator);
	value.AddMember("stiffness", stiffness, allocator);
	value.AddMember("damping", damping, allocator);
	PointJoint::ToJSON(value, allocator);
}
bool WeldJoint::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "stiffness", stiffness) &&
		jsonutils::GetMember(value, "damping", damping) &&
		PointJoint::FromJSON(value);
}


