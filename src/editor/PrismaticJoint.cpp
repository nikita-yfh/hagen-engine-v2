#include "PrismaticJoint.hpp"
#include "GLUtils.hpp"
#include <wx/glcanvas.h>

PrismaticJoint::PrismaticJoint() :
		enableLimit(false), lowerTranslation(-3.0f), upperTranslation(3.0f),
		enableMotor(false), maxMotorForce(1000.0f), motorSpeed(10.0f) {}

void PrismaticJoint::Draw(const Colors &colors) const{
	ApplyDrawJoint(colors);
	DrawConnections(colors,position);
	Transform();
	glBegin(GL_LINES);
	if(enableLimit){
		glVertex2f(lowerTranslation,0.0f);
		glVertex2f(upperTranslation,0.0f);
	}else{
		glVertex2f(-10000.0f,0.0f);
		glVertex2f( 10000.0f,0.0f);
	}
	glEnd();
}
void PrismaticJoint::DrawPoints(const Colors &colors) const{
	DrawPoint(colors,1,position);
	if(enableLimit){
		Transform();
		DrawPoint(colors,2,b2Vec2(lowerTranslation,0.0f));
		DrawPoint(colors,3,b2Vec2(upperTranslation,0.0f));
	}
}
bool PrismaticJoint::UpdatePoints(const Mouse &_mouse){
	if(UpdatePoint(_mouse,1,position))
		return true;
	if(enableLimit){
		Mouse mouse=GetLocalMouse(_mouse);
		b2Vec2 lowerVec(lowerTranslation,0.0f);
		b2Vec2 upperVec(upperTranslation,0.0f);
		if(UpdatePoint(mouse,2,lowerVec,false)){
			lowerTranslation=-mouse.camera.ToGrid(lowerVec.Length());
			angle+=mouse.camera.AngleToGrid(glutils::VecToAngle(lowerVec));
			return true;
		}
		if(UpdatePoint(mouse,3,upperVec,false)){
			upperTranslation=mouse.camera.ToGrid(upperVec.Length());
			angle+=mouse.camera.AngleToGrid(glutils::VecToAngle2(lowerVec));
			return true;
		}
	}
	return false;
}
bool PrismaticJoint::Create(const Mouse &mouse){
	UpdatePoint(mouse,1,position);
	if(selected==2)
		angle=mouse.camera.AngleToGrid(glutils::VecToAngle(mouse.position-position));
	if(mouse.pressed){
		if(selected==1)
			selected=2;
		else if(selected==2)
			return true;
	}
	return false;
}

void PrismaticJoint::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	Rotatable::UpdatePropertyGrid(pg, n);
	if(!n){
		pg->GetProperty("LowerTranslation")->SetValue(lowerTranslation);
		pg->GetProperty("UpperTranslation")->SetValue(upperTranslation);
	}else{
		pg->Append(new wxBoolProperty("EnableLimit", wxPG_LABEL, enableLimit));
		pg->Append(new wxFloatProperty("LowerTranslation", wxPG_LABEL, lowerTranslation));
		pg->Append(new wxFloatProperty("UpperTranslation", wxPG_LABEL, upperTranslation));
		pg->Append(new wxBoolProperty("EnableMotor", wxPG_LABEL, enableMotor));
		pg->Append(new wxFloatProperty("MaxMotorForce", wxPG_LABEL, maxMotorForce));
		pg->Append(new wxFloatProperty("MotorSpeed", wxPG_LABEL, motorSpeed));
	}
	Joint::UpdatePropertyGrid(pg, n);
}
void PrismaticJoint::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	Rotatable::OnPropertyGridChange(name, value);
	if(name == "EnableLimit")
		enableLimit = value.GetBool();
	else if(name == "LowerTranslation")
		lowerTranslation = value.GetDouble();
	else if(name == "UpperTranslation")
		upperTranslation = value.GetDouble();
	else if(name == "EnableMotor")
		enableMotor = value.GetBool();
	else if(name == "MaxMotorForce")
		maxMotorForce = value.GetDouble();
	else if(name == "MotorSpeed")
		motorSpeed = value.GetDouble();
	Joint::OnPropertyGridChange(name, value);
}
