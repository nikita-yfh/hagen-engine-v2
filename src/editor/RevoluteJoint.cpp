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
		pg->Append(new wxBoolProperty("EnableLimit", wxPG_LABEL, enableLimit));
		pg->Append(new wxFloatProperty("LowerAngle", wxPG_LABEL, lowerAngle));
		pg->Append(new wxFloatProperty("UpperAngle", wxPG_LABEL, upperAngle));
		pg->Append(new wxBoolProperty("EnableMotor", wxPG_LABEL, enableMotor));
		pg->Append(new wxFloatProperty("MaxMotorTorque", wxPG_LABEL, maxMotorTorque));
		pg->Append(new wxFloatProperty("MotorSpeed", wxPG_LABEL, motorSpeed));
	}
	PointJoint::UpdatePropertyGrid(pg, n);
}
void RevoluteJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "EnableLimit")
		enableLimit = value.GetBool();
	else if(name == "LowerAngle")
		lowerAngle = value.GetDouble();
	else if(name == "UpperAngle")
		upperAngle = value.GetDouble();
	else if(name == "EnableMotor")
		enableMotor = value.GetBool();
	else if(name == "MaxMotorTorque")
		maxMotorTorque = value.GetDouble();
	else if(name == "MotorSpeed")
		motorSpeed = value.GetDouble();
	PointJoint::OnPropertyGridChange(name, value);
}
