#include "EditMenu.hpp"
#include "EditorMain.hpp"

EditMenu::EditMenu(){
	{
		wxMenu *menuAddBody=new wxMenu();
		menuAddBody->Append(ID_ADD_BODY_STATIC,		_("Static\tCtrl-Shift-S"));
		menuAddBody->Append(ID_ADD_BODY_KINEMATIC,	_("Kinematic\tCtrl-Shift-K"));
		menuAddBody->Append(ID_ADD_BODY_DYNAMIC,	_("Dynamic\tCtrl-Shift-D"));
		Append(wxID_ADD,_("Add body"),menuAddBody);
	}
	{
		wxMenu *menuAddFixture=new wxMenu();
		menuAddFixture->Append(ID_ADD_FIXTURE_CIRCLE,	_("Circle\tCtrl-Shift-C"));
		menuAddFixture->Append(ID_ADD_FIXTURE_EDGE,		_("Edge\tCtrl-Shift-E"));
		menuAddFixture->Append(ID_ADD_FIXTURE_POLYGON,	_("Polygon\tCtrl-Shift-P"));
		menuAddFixture->Append(ID_ADD_FIXTURE_CHAIN,	_("Chain\tCtrl-Shift-L"));
		Append(ID_ADD_FIXTURE,_("Add fixture"),menuAddFixture);
	}
	{
		wxMenu *menuAddJoint=new wxMenu();
		menuAddJoint->Append(ID_ADD_JOINT_REVOLUTE,		_("Revolute"));
		menuAddJoint->Append(ID_ADD_JOINT_PRISMATIC,	_("Prismatic"));
		menuAddJoint->Append(ID_ADD_JOINT_DISTANCE,		_("Distance"));
		menuAddJoint->Append(ID_ADD_JOINT_PULLEY,		_("Pulley"));
		menuAddJoint->Append(ID_ADD_JOINT_GEAR,			_("Gear"));
		menuAddJoint->Append(ID_ADD_JOINT_WHEEL,		_("Wheel"));
		menuAddJoint->Append(ID_ADD_JOINT_WELD,			_("Weld"));
		menuAddJoint->Append(ID_ADD_JOINT_FRICTION,		_("Friction"));
		menuAddJoint->Append(ID_ADD_JOINT_ROPE,			_("Rope"));
		menuAddJoint->Append(ID_ADD_JOINT_MOTOR,		_("Motor"));
		Append(ID_ADD_JOINT,_("Connect by joint"),menuAddJoint);
	}
	AppendSeparator();
	Append(wxID_COPY,		_("Copy\tCtrl-C"));
	Append(wxID_CUT,		_("Cut\tCtrl-X"));
	Append(wxID_PASTE,		_("Paste\tCtrl-V"));
	Append(wxID_DUPLICATE,	_("Duplicate\tCtrl-D"));
	Append(wxID_DELETE,		_("Delete\tDelete"));
	AppendSeparator();
	Append(wxID_SELECTALL,	_("Select all\tCtrl-A"));
}
