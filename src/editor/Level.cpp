#include "Level.hpp"

#include "Circle.hpp"
#include "Edge.hpp"
#include "Poly.hpp"
#include "Chain.hpp"

#include "WeldJoint.hpp"
#include "RevoluteJoint.hpp"
#include "PrismaticJoint.hpp"
#include "DistanceJoint.hpp"
#include "PulleyJoint.hpp"

#include <wx/glcanvas.h>

Level::Level(const Directory &dir) :
	textures(nullptr),
	objects(nullptr),
	create(nullptr),
	gravity(0.0f, -9.8f),
	allowSleep(true),
	textureScale(100.0f),
	gameDir(dir) {}

void Level::Clear(){
	while(textures){
		Texture *child = textures->next;
		delete child;
		textures = child;
	}
	while(objects){
		Object *child = objects->next;
		delete objects;
		objects = child;
	}
	gravity.Set(0.0f, -9.8f);
	textureScale = 100.0f;
}
Texture *Level::GetTextureByID(const wxString &id){
	for(Texture *texture = textures; texture; texture = texture->next)
		if(texture->GetName() == id)
			return texture;
	return nullptr;
}

void Level::AddImage(const wxString &path){
	wxString name = gameDir.ConvertToLocalPath(path);
	Texture *texture = GetTextureByID(name);
	if(!texture)
		return;
	const Object *selected = GetFirstSelected();
	if(selected && selected->GetObjectType() != Object::BODY)
		selected = nullptr;
	Image *image = new Image(texture, (const Body*)selected, textureScale);
	AddObject(image);
}
void Level::AddJoint(Joint *joint){
	Body *a = nullptr;
   	Body *b = nullptr;
	for(Object *object = objects;object;object = object->next){
		if(object->IsSelected() && object->GetObjectType() == Object::BODY){
			if(a)
				b = (Body*)object;
			else
				a = (Body*)object;
		}
	}
	if(!a || !b)
		return;
	joint->SetBodies(a, b);
	AddObject(joint);
}
void Level::AddObject(Object *object){
	UnselectAll();
	object->next = objects;
	object->id = GetFreeID();
	create = objects = object;
}
void Level::AddLoadObject(Object *object){
	if(!objects)
		objects = object;
	else{
		Object *last = objects;
		while(last->next)
			last = last->next;
		last->next = object;
	}
}
void Level::DeleteDeps(const void *object){
	UnselectAll();
	SelectDeps(object);
	DeleteSelected();
}
bool Level::SelectDeps(const void *del){
	bool ok = false;
	for(Object *object = objects;object;object = object->next)
		if(!object->TryRemove(del)){
			object->Select();
			ok = true;
		}
	return ok;
}
Texture *Level::AddTexture(const wxString &name){
	Texture *temp = textures;
	textures = new Texture(gameDir, name);
	textures->next = temp;
	return textures;
}
void Level::DeleteTexture(const wxString &name){
	Texture *del = nullptr;
	if(textures && textures->GetName() == name){
		del = textures;
		textures = textures->next;
	}else{
		for(Texture *texture = textures; texture; texture = texture->next){
			Texture *next = texture->next;
			if(next && next->GetName() == name){
				texture->next = next->next;
				break;
			}
		}
	}
	if(!del)
		return;
	DeleteDeps(del);
	delete del;
}
void Level::Draw(const Colors &colors) const{
	for(int layer = INT8_MIN; layer <= INT8_MAX; layer++){
		for(Object *object = objects;object;object = object->next){
			if(object->GetLayer() == layer){
				glPushMatrix();
				object->Draw(colors);
				glPopMatrix();
			}
		}
	}
}
void Level::DrawPoints(const Colors &colors) const{
	for(Object *object = objects;object;object = object->next){
		glPushMatrix();
		object->DrawPoints(colors);
		glPopMatrix();
	}
}
bool Level::UpdatePoints(const Mouse &mouse){
	if(create){
		if(create->Create(mouse)){
			create->Select();
			create = nullptr; //created yet
		}
		return true;
	}else{
		for(int layer = INT8_MAX; layer >= INT8_MIN; layer--)
			for(Object *object = objects;object;object = object->next)
				if(object->GetLayer() == layer && object->UpdatePoints(mouse))
					return true;
	}
	return false;
}
void Level::UnselectAllPoints(){
	for(Object *object = objects;object;object = object->next)
		object->UnselectPoints();
}

void Level::SelectAll(){
	for(Object *object = objects;object;object = object->next)
		object->Select();
}
void Level::UnselectAll(){
	for(Object *object = objects;object;object = object->next)
		object->Unselect();
}

int Level::GetSelectedBodyCount() const{
	int count = 0;
	for(Object *object = objects;object;object = object->next)
		if(object->IsSelected() && object->GetObjectType() == Object::BODY)
			count++;
	return count;
}
int Level::GetSelectedNotDynamicBodyCount() const{
	int count = 0;
	for(Object *object = objects;object;object = object->next)
		if(object->IsSelected() && object->GetObjectType() == Object::BODY && ((Body*)object)->GetType() != b2_dynamicBody)
			count++;
	return count;
}
int Level::GetSelectedJointCount() const{
	int count = 0;
	for(Object *object = objects;object;object = object->next)
		if(object->IsSelected() && object->GetObjectType() == Object::JOINT)
			count++;
	return count;
}
int Level::GetSelectedCount() const{
	int count = 0;
	for(Object *object = objects;object;object = object->next)
		if(object->IsSelected())
			count++;
	return count;
}
Object *Level::GetFirstSelected(){
	for(Object *object = objects;object;object = object->next)
		if(object->IsSelected())
			return object;
	return nullptr;
}
Object *Level::GetFirstSelectedAll(){
	Object *selected = GetFirstSelected();
	if(selected)
		return selected;
	return this;
}

void Level::AddFixture(Fixture *fixture){
	Body *selected = nullptr;
	for(Object *object = objects;object;object = object->next){
		if(object->IsSelected() && object->GetObjectType() == Object::BODY){
			selected = (Body*)object;
			break;
		}
	}
	if(selected == nullptr)
		return;
	UnselectAll();
	//Add fixture after body
	fixture->SetParent(selected);
	fixture->next = selected->next;
	fixture->id = GetFreeID();
	create = selected->next = fixture;
}
bool Level::CancelCreating(){
	create->Select();
	if(!create->CancelCreating())
		DeleteSelected();
	create = nullptr;
	return true;
}
void Level::DeleteSelected(){
	Object *prev = nullptr;
	bool ok;
	do{
		ok = false;
		for(Object *object = objects; object;){
			if(object->IsSelected()){
				if(SelectDeps(object))
					ok = true;
				if(prev)
					prev->next = object->next;
				else
					objects = object->next;
				Object *child = object->next;
				delete object;
				object = child;
			}else{
				prev = object;
				object = object->next;
			}
		}
	}while(ok);
}
void Level::ReloadTextures(){
	for(Texture *tex = textures; tex; tex = tex->next)
		tex->Reload(gameDir);
}
void Level::ResolveID(Object *object) const{
	if(!IsFreeID(object->id, object))
		object->id = GetFreeID();
}
Body *Level::GetBodyByID(const wxString &id) {
	for(Object *object = objects; object; object = object->next)
		if(object->GetObjectType() == BODY && object->id == id)
			return (Body*)object;
	return nullptr;
}
bool Level::IsFreeID(const wxString &id, const Object *sel) const{
	for(const Object *object = objects; object; object = object->next)
		if(object != sel && object->id == id)
			return false;
	return true;
}
wxString Level::GetFreeID() const{
	for(int n = 0;; n++){
		wxString id(wxString::Format("Object%d", n));
		if(IsFreeID(id))
			return id;
	}
}

void Level::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n){
		pg->Append(new Vec2Property("gravity", wxPG_LABEL, gravity));
		pg->Append(new wxFloatProperty("textureScale", wxPG_LABEL, textureScale));
	}
}
void Level::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "gravity")
		gravity << value;
	else if(name == "textureScale")
		textureScale = value.GetLong();
}
void Level::Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const {
	value.AddMember("gravity", jsonutils::Value(gravity, allocator), allocator);
	value.AddMember("textureScale", textureScale, allocator);
	rapidjson::Value bodyArray(rapidjson::kArrayType);
	rapidjson::Value jointArray(rapidjson::kArrayType);
	rapidjson::Value imageArray(rapidjson::kArrayType);
	for(Object *object = objects; object; object = object->next){ rapidjson::Value j(rapidjson::kObjectType);
		switch(object->GetObjectType()){
		case Object::BODY:{
			((Body*)object)->Save(j, allocator);
			rapidjson::Value fixtureArray(rapidjson::kArrayType);
			while(object->next && object->next->GetObjectType() == Object::FIXTURE){
				rapidjson::Value f(rapidjson::kObjectType);
				((Fixture*)object->next)->Save(f, allocator);
				fixtureArray.PushBack(f, allocator);
				object = object->next;
			}
			j.AddMember("fixtures", fixtureArray, allocator);
			bodyArray.PushBack(j, allocator);
			break;
		}
		case Object::IMAGE:
			((Image*)object)->Save(j, allocator);
			imageArray.PushBack(j, allocator);
			break;
		case Object::JOINT:
			((Joint*)object)->Save(j, allocator);
			jointArray.PushBack(j, allocator);
		}
	}
	value.AddMember("bodies", bodyArray, allocator);
	value.AddMember("joints", jointArray, allocator);
	value.AddMember("images", imageArray, allocator);
}
bool Level::Load(const rapidjson::Value &value){
	Clear();
	if(jsonutils::GetMember(value, "gravity", gravity) ||
	   jsonutils::GetMember(value, "textureScale", textureScale))
		return true;
	if(value.HasMember("bodies")){
		const rapidjson::Value &bodyArray = value["bodies"];
		if(!bodyArray.IsArray())
			return true;
		for(int i=0; i<bodyArray.Size(); i++){
			const rapidjson::Value &b = bodyArray[i];
			Body *body = new Body();
			AddLoadObject(body);
			if(body->Load(b))
				return true;
			if(!b.HasMember("fixtures"))
				return true;
			const rapidjson::Value &fixtureArray = b["fixtures"];
			if(!fixtureArray.IsArray())
				return true;
			for(int j=0; j<fixtureArray.Size(); j++){
				const rapidjson::Value &f = fixtureArray[j];
				const char *ctype;
				if(jsonutils::GetMember(f, "type", ctype))
					return true;
				Fixture *fixture;
				if(strcmp(ctype, "circle") == 0)
					fixture = new Circle();
				else if(strcmp(ctype, "edge") == 0)
					fixture = new Edge();
				else if(strcmp(ctype, "polygon") == 0)
					fixture = new Poly();
				else if(strcmp(ctype, "chain") == 0)
					fixture = new Chain();
				else return true;
				fixture->SetParent(body);
				AddLoadObject(fixture);
				if(fixture->Load(f))
					return true;
			}
		}
	}
	if(value.HasMember("joints")){
		const rapidjson::Value &jointArray = value["joints"];
		if(!jointArray.IsArray())
			return true;
		for(int i=0; i<jointArray.Size(); i++){
			const rapidjson::Value &j = jointArray[i];
			wxString bodyIDA, bodyIDB;
			if(jsonutils::GetMember(j, "bodyA", bodyIDA) ||
			   jsonutils::GetMember(j, "bodyB", bodyIDB))
				return true;
			Body *bodyA = GetBodyByID(bodyIDA);
			Body *bodyB = GetBodyByID(bodyIDB);
			if(!bodyA || !bodyB)
				return true;
			Joint *joint;
			const char *ctype;
			if(jsonutils::GetMember(j, "type", ctype))
				return true;
			if(strcmp(ctype, "weld") == 0)
				joint = new WeldJoint();
			else if(strcmp(ctype, "revolute") == 0)
				joint = new RevoluteJoint();
			else if(strcmp(ctype, "prismatic") == 0)
				joint = new PrismaticJoint();
			else if(strcmp(ctype, "distance") == 0)
				joint = new DistanceJoint();
			else if(strcmp(ctype, "pulley") == 0)
				joint = new PulleyJoint();
			else return true;
			AddLoadObject(joint);
			if(joint->Load(j))
				return true;
			joint->SetBodies(bodyA, bodyB);
		}
	}
	if(value.HasMember("images")){
		const rapidjson::Value &imageArray = value["images"];
		if(!imageArray.IsArray())
			return true;
		for(int i=0; i<imageArray.Size(); i++){
			const rapidjson::Value &j = imageArray[i];
			wxString bindID, textureID;
			if(jsonutils::GetMember(j, "bindBody", bindID) ||
			   jsonutils::GetMember(j, "texture", textureID))
				return true;
			Body *bind = GetBodyByID(bindID);
			Texture *texture = AddTexture(textureID);
			if(!bind)
				return true;
			Image *image = new Image(texture, bind, textureScale);
			AddLoadObject(image);
			if(image->Load(j))
				return true;
		}
	}
	UnselectAll();
	return false;
}
