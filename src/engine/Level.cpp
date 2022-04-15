#include "Level.hpp"

static const char *bodyTypes[] = {
	"static",
	"kinematic",
	"dynamic"
};

static bool LoadFilter(b2Filter *filter, const rapidjson::Value &value) {
	return
		jsonutils::CheckObject(value) &&
		jsonutils::GetMember(value, "category", filter->categoryBits) && 
		jsonutils::GetMember(value, "mask", filter->maskBits) && 
		jsonutils::GetMember(value, "group", filter->groupIndex);
}

static bool LoadCircleShape(b2CircleShape *shape, const rapidjson::Value &value) {
	return
		jsonutils::GetMember(value, "position", shape->m_p) &&
		jsonutils::GetMember(value, "radius", shape->m_radius);
}
static bool LoadEdgeShape(b2EdgeShape *shape, const rapidjson::Value &value) {
	return
		jsonutils::GetMember(value, "point1", shape->m_vertex1) &&
		jsonutils::GetMember(value, "point2", shape->m_vertex2);
}
static bool LoadPolygonShape(b2PolygonShape *shape, const rapidjson::Value &value) {
	if(!jsonutils::CheckValue(value, "points"))
		return false;
	const rapidjson::Value &array = value["points"];
	if(!jsonutils::CheckArray(array, 3, b2_maxPolygonVertices))
		return false;
	b2Vec2 points[b2_maxPolygonVertices];
	for(int i = 0; i < array.Size(); i++)
		if(!jsonutils::GetArrayMember(array, i, points[i]))
			return false;
	shape->Set(points, array.Size());
	return true;
}
static bool LoadChainShape(b2ChainShape *shape, const rapidjson::Value &value) {
	if(!jsonutils::CheckValue(value, "points"))
		return false;
	const rapidjson::Value &array = value["points"];
	if(!jsonutils::CheckArray(array, 2, SIZE_MAX))
		return false;
	b2Vec2 *points = new b2Vec2[array.Size()];
	for(int i = 0; i < array.Size(); i++)
		if(!jsonutils::GetArrayMember(array, i, points[i])){
			delete [] points;
			return false;
		}
	shape->CreateChain(points, array.Size(), points[0], points[array.Size() -1]);
	delete [] points;
	return true;
}

static bool LoadFixture(b2Body *body, const rapidjson::Value &value) {
	b2FixtureDef def;
	const char *ctype;
	const char *id;
	if(!jsonutils::CheckObject(value) || !jsonutils::CheckValue(value, "filter"))
		return false;
	const rapidjson::Value &filter = value["filter"];
	if(!LoadFilter(&def.filter, filter) ||
		!jsonutils::GetMember(value, "type",  ctype) ||
		!jsonutils::GetMember(value, "id", id) ||
		!jsonutils::GetMember(value, "friction", def.friction) ||
		!jsonutils::GetMember(value, "density", def.density) ||
		!jsonutils::GetMember(value, "restitution", def.restitution) ||
		!jsonutils::GetMember(value, "restitutionThreshold", def.restitutionThreshold) ||
		!jsonutils::GetMember(value, "isSensor", def.isSensor))
		return false;
	def.userData.hash = String::Hash(id);
	if(strcmp(ctype, "circle") == 0) {
		b2CircleShape shape;
		if(!LoadCircleShape(&shape, value))
			return false;
		def.shape = &shape;
		body->CreateFixture(&def);
	} else if(strcmp(ctype, "edge") == 0) {
		b2EdgeShape shape;
		if(!LoadEdgeShape(&shape, value))
			return false;
		def.shape = &shape;
		body->CreateFixture(&def);
	} else if(strcmp(ctype, "polygon") == 0) {
		b2PolygonShape shape;
		if(!LoadPolygonShape(&shape, value))
			return false;
		def.shape = &shape;
		body->CreateFixture(&def);
	} else if(strcmp(ctype, "chain") == 0) {
		b2ChainShape shape;
		if(!LoadChainShape(&shape, value))
			return false;
		def.shape = &shape;
		body->CreateFixture(&def);
	}
	return true;
}
static bool LoadBodyDef(b2BodyDef *def, const rapidjson::Value &value) {
	const char *ctype;
	const char *id;
	if(!jsonutils::CheckObject(value) || !jsonutils::GetMember(value, "id", id))
		return false;
	def->userData.hash = String::Hash(id);
	return
		jsonutils::GetMember(value, "type", ctype) &&
		(def->type = (b2BodyType)jsonutils::GetEnum(ctype, bodyTypes, 3)) != -1 &&
		jsonutils::GetMember(value, "position", def->position) &&
		jsonutils::GetMember(value, "angle", def->angle) &&
		jsonutils::GetMember(value, "linearVelocity", def->linearVelocity) &&
		jsonutils::GetMember(value, "angularVelocity", def->angularVelocity) &&
		jsonutils::GetMember(value, "linearDamping", def->linearDamping) &&
		jsonutils::GetMember(value, "angularDamping", def->angularDamping) &&
		jsonutils::GetMember(value, "allowSleep", def->allowSleep) &&
		jsonutils::GetMember(value, "awake", def->awake) &&
		jsonutils::GetMember(value, "fixedRotation", def->fixedRotation) &&
		jsonutils::GetMember(value, "bullet", def->bullet) &&
		jsonutils::GetMember(value, "enabled", def->enabled) &&
		jsonutils::GetMember(value, "gravityScale", def->gravityScale);
}
Image *Level::LoadImage(const rapidjson::Value &value) {
	Image *image = new Image();
	image->next = images;
	image->prev = nullptr;
	if(images)
		images->next = image;
	images = image;
	const char *id;
	const char *body;
	const char *texture;
	if(!jsonutils::CheckObject(value) ||
		!jsonutils::GetMember(value, "id", id) ||
		!jsonutils::GetMember(value, "bindBody", body) ||
		!jsonutils::GetMember(value, "texture", texture) ||
		!jsonutils::GetMember(value, "position", image->position) ||
		!jsonutils::GetMember(value, "angle", image->angle) ||
		!jsonutils::GetMember(value, "layer", image->layer) ||
		!jsonutils::GetMember(value, "repeat", image->repeat))
		return nullptr;
	image->hash = String::Hash(id);
	image->bindBody = FindBody(body);
	image->texture = resManager.LoadResource<Texture>(texture);
	return image;
}


Level::Level(ResourceManager &_resManager)
	: resManager(_resManager), b2World(b2Vec2_zero) {}

b2Body *Level::LoadBody(const rapidjson::Value &value) {
	b2BodyDef bodyDef;
	if(!LoadBodyDef(&bodyDef, value))
		return nullptr;
	b2Body *body = CreateBody(&bodyDef);
	if(!jsonutils::CheckValue(value, "fixtures"))
		return nullptr;
	const rapidjson::Value &fixtures = value["fixtures"];
	if(!jsonutils::CheckArray(fixtures))
		return nullptr;
	for(int i = 0; i < fixtures.Size(); i++) {
		if(!LoadFixture(body, fixtures[i]))
			return nullptr;
	}
	return body;
}
bool Level::FromJSON(const rapidjson::Value &value) {
	b2Vec2 gravity;
	if(!jsonutils::CheckObject(value) ||
		!jsonutils::GetMember(value, "gravity", gravity) ||
		!jsonutils::GetMember(value, "textureScale", textureScale) ||
		!jsonutils::CheckValue(value, "bodies") ||
		!jsonutils::CheckValue(value, "joints") ||
		!jsonutils::CheckValue(value, "images"))
		return false;
	SetGravity(gravity);
	const rapidjson::Value &bodies = value["bodies"];
	const rapidjson::Value &joints = value["joints"];
	const rapidjson::Value &images = value["images"];
	for(int i = 0; i < bodies.Size(); i++)
		if(!LoadBody(bodies[i]))
			return false;
	return true;
}
b2Body *Level::FindBody(const char *id) {
	uint32_t hash = String::Hash(id);
	for(b2Body *body = GetBodyList(); body; body = body->GetNext())
		if(body->GetUserData().hash == hash)
			return body;
	return nullptr;
}
b2Joint *Level::FindJoint(const char *id) {
	uint32_t hash = String::Hash(id);
	for(b2Joint *joint = GetJointList(); joint; joint = joint->GetNext())
		if(joint->GetUserData().hash == hash)
			return joint;
	return nullptr;
}
void Level::Render() const {
	camera.Apply();
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();
}
	
