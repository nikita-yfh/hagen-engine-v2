#include "Level.hpp"
#include <wx/glcanvas.h>

Level::Level() :
	bodies(nullptr),
	joints(nullptr),
	create(nullptr),
	gravity(0.0f, -9.8f),
	allowSleep(true) {}

void Level::Clear(){
	while(bodies){
		Body *child=bodies->next;
		delete bodies;
		bodies=child;
	}
	while(joints){
		Joint *child=joints->next;
		delete joints;
		joints=child;
	}
}
Level::~Level(){
	Clear();
}
void Level::AddBody(Body *body){
	UnselectAll();
	body->next=bodies;
	create=bodies=body;
}
void Level::AddJoint(Joint *joint){
	joint->SetBodies(GetFirstSelectedBody(),
					GetSecondSelectedBody());
	UnselectAll();
	joint->next=joints;
	create=joints=joint;
}
void Level::Draw(const Colors &colors) const{
	for(Body *body=bodies;body;body=body->next)
		body->Draw(colors);
	for(Joint *joint=joints;joint;joint=joints->next)
		joints->Draw(colors);
}
void Level::DrawPoints(const Colors &colors) const{
	for(Body *body=bodies;body;body=body->next)
		body->DrawPoints(colors);
	for(Joint *joint=joints;joint;joint=joints->next)
		joints->DrawPoints(colors);
}
bool Level::UpdatePoints(const Mouse &mouse){
	if(create){
		if(create->Create(mouse)){
			create->Select();
			create=nullptr; //created yet
		}
		return true;
	}else{
		for(Joint *joint=joints;joint;joint=joint->next)
			if(joint->UpdatePoints(mouse))
				return true;
		for(Body *body=bodies;body;body=body->next)
			if(body->UpdatePoints(mouse))
				return true;
	}
	return false;
}
void Level::UnselectAllPoints(){
	for(Body *body=bodies;body;body=body->next)
		body->UnselectPoints();
	for(Joint *joint=joints;joint;joint=joint->next)
		joint->UnselectPoints();
}

void Level::SelectAll(){
	for(Body *body=bodies;body;body=body->next)
		body->SelectAll();
	for(Joint *joint=joints;joint;joint=joint->next)
		joint->Select();
}
void Level::UnselectAll(){
	for(Body *body=bodies;body;body=body->next)
		body->UnselectAll();
	for(Joint *joint=joints;joint;joint=joint->next)
		joint->Unselect();
}

Body *Level::GetFirstSelectedBody(){
	for(Body *body=bodies; body; body=body->next)
		if(body->IsSelected())
			return body;
	return nullptr;
}
Body *Level::GetSecondSelectedBody(){
	Body *first=nullptr;
	for(Body *body=bodies; body; body=body->next)
		if(body->IsSelected()){
			if(first)
				return body;
			else
				first=body;
		}
	return nullptr;
}
int Level::GetSelectedBodyCount() const{
	int count=0;
	for(Body *body=bodies;body;body=body->next)
		if(body->IsSelected())
			count++;
	return count;
}
int Level::GetSelectedNotDynamicBodyCount() const{
	int count=0;
	for(Body *body=bodies;body;body=body->next)
		if(body->IsSelected() && body->type != b2_dynamicBody)
			count++;
	return count;
}
int Level::GetSelectedJointCount() const{
	int count=0;
	for(Joint *joint=joints;joint;joint=joint->next)
		if(joint->IsSelected())
			count++;
	return count;
}
int Level::GetSelectedCount() const{
	int count=GetSelectedJointCount();
	for(Body *body=bodies;body;body=body->next)
		count+=body->GetSelectedCount();
	return count;
}
Object *Level::GetFirstSelected(){
	for(Joint *joint=joints;joint;joint=joint->next)
		if(joint->IsSelected())
			return joint;
	for(Body *body=bodies;body;body=body->next){
		Object *obj=body->GetFirstSelected();
		if(obj)
			return obj;
	}
	return nullptr;
}
Object *Level::GetFirstSelectedAll(){
	Object *selected=GetFirstSelected();
	if(selected)
		return selected;
	return this;
}

void Level::AddFixture(Fixture *fixture){
	Body *body=GetFirstSelectedBody();
	if(body==nullptr)
		return;
	UnselectAll();
	body->AddFixture(fixture);
	create=fixture;
}
bool Level::IsCreating() const{
	return create!=nullptr;
}
bool Level::CancelCreating(){
	create->Select();
	if(!create->CancelCreating())
		DeleteSelected();
	create=nullptr;
	return true;
}
void Level::DeleteSelectedBodies(){
	Body *prev=nullptr;
	for(Body *body=bodies; body;){
		if(body->IsSelected()){
			for(Joint *joint=joints; joint; joint=joint->next)
				if(joint->HasBody(body))
					joint->Select(); //for delete
			if(prev)
				prev->next=body->next;
			else
				bodies=body->next;
			Body *child=body->next;
			delete body;
			body=child;
		}else{
			body->DeleteSelected();
			prev=body;
			body=body->next;
		}
	}
}
void Level::DeleteSelectedJoints(){
	Joint *prev=nullptr;
	for(Joint *joint=joints; joint;){
		if(joint->IsSelected()){
			if(prev)
				prev->next=joint->next;
			else
				joints=joint->next;
			Joint *child=joint->next;
			delete child;
			joint=child;
		}else{
			prev=joint;
			joint=joint->next;
		}
	}
}

void Level::DeleteSelected(){
	DeleteSelectedBodies();
	DeleteSelectedJoints();
}

void Level::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n)
		pg->Append(new Vec2Property("Gravity", wxPG_LABEL, gravity));
}
void Level::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "Gravity")
		gravity << value;
}
