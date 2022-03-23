#include "Level.hpp"
#include <wx/glcanvas.h>

Level::Level(const Directory &dir) :
	textures(nullptr),
	objects(nullptr),
	create(nullptr),
	gravity(0.0f, -9.8f),
	allowSleep(true),
	textureScale(100.0f),
	gameDir(dir) {}

const Directory &Level::GetGameDir() const{
	return gameDir;
}
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
}
Level::~Level(){
	Clear();
}
void Level::AddImage(const wxString &path){
	wxString name = gameDir.ConvertToLocalPath(path);
	for(Texture *texture = textures; texture; texture = texture->next){
		if(texture->name == name){
			const Object *selected = GetFirstSelected();
			if(selected && selected->GetObjectType() != Object::BODY)
				selected = nullptr;
			Image *image = new Image(texture, (const Body*)selected, textureScale);
			AddObject(image);
			return;
		}
	}
}
void Level::AddBody(Body *body){
	AddObject(body);
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
	create = objects = object;
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
void Level::AddTexture(const wxString &name){
	Texture *temp = textures;
	textures = new Texture(gameDir, name);
	textures->next = temp;
}
void Level::DeleteTexture(const wxString &name){
	Texture *del = nullptr;
	if(textures && textures->name == name){
		del = textures;
		textures = textures->next;
	}else{
		for(Texture *texture = textures; texture; texture = texture->next){
			Texture *next = texture->next;
			if(next && next->name == name){
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
	create = selected->next = fixture;
}
bool Level::IsCreating() const{
	return create!=nullptr;
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
void Level::UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const{
	if(n){
		pg->Append(new Vec2Property("Gravity", wxPG_LABEL, gravity));
		pg->Append(new wxFloatProperty("TextureScale", wxPG_LABEL, textureScale));
	}
}
void Level::OnPropertyGridChange(const wxString &name, const wxVariant &value){
	if(name == "Gravity")
		gravity << value;
	else if(name == "TextureScale")
		textureScale = value.GetLong();
}
