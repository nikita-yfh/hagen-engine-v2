#include "RevoluteJoint.hpp"

RevoluteJoint::RevoluteJoint() :
	enableLimit(false),
	lowerAngle(0.0f),
	upperAngle(0.0f),
	enableMotor(false),
	maxMotorTorque(0.0f),
	motorSpeed(0.0f) {}

void RevoluteJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n){
		pg->Append(new wxBoolProperty("enableLimit", wxPG_LABEL, enableLimit));
		pg->Append(new wxFloatProperty("lowerAngle", wxPG_LABEL, lowerAngle));
		pg->Append(new wxFloatProperty("upperAngle", wxPG_LABEL, upperAngle));
		pg->Append(new wxBoolProperty("enableMotor", wxPG_LABEL, enableMotor));
		pg->Append(new wxFloatProperty("maxMotorTorque", wxPG_LABEL, maxMotorTorque));
		pg->Append(new wxFloatProperty("motorSpeed", wxPG_LABEL, motorSpeed));
	}
	PointJoint::UpdatePropertyGrid(pg, n);
}
void RevoluteJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "enableLimit")
		enableLimit = value.GetBool();
	else if(name == "lowerAngle")
		lowerAngle = value.GetDouble();
	else if(name == "upperAngle")
		upperAngle = value.GetDouble();
	else if(name == "enableMotor")
		enableMotor = value.GetBool();
	else if(name == "maxMotorTorque")
		maxMotorTorque = value.GetDouble();
	else if(name == "motorSpeed")
		motorSpeed = value.GetDouble();
	PointJoint::OnPropertyGridChange(name, value);
}
void RevoluteJoint::ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	value.AddMember("type", "revolute", allocator);
	value.AddMember("enableLimit", enableLimit, allocator);
	value.AddMember("lowerAngle", lowerAngle, allocator);
	value.AddMember("upperAngle", upperAngle, allocator);
	value.AddMember("enableMotor", enableMotor, allocator);
	value.AddMember("maxMotorTorque", maxMotorTorque, allocator);
	value.AddMember("motorSpeed", motorSpeed, allocator);
	PointJoint::ToJSON(value, allocator);
}
bool RevoluteJoint::FromJSON(const rapidjson::Value &value){
	return
		jsonutils::GetMember(value, "enableLimit", enableLimit) &&
		jsonutils::GetMember(value, "lowerAngle", lowerAngle) &&
		jsonutils::GetMember(value, "upperAngle", upperAngle) &&
		jsonutils::GetMember(value, "enableMotor", enableMotor) &&
		jsonutils::GetMember(value, "maxMotorTorque", maxMotorTorque) &&
		jsonutils::GetMember(value, "motorSpeed", motorSpeed) &&
		PointJoint::FromJSON(value);
}


