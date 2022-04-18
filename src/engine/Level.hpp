#pragma once

#include "ResourceManager.hpp"
#include "Image.hpp"
#include "Camera.hpp"
#include "JSONUtils.hpp"
#include "glad.h"
#include "box2d.h"

class Level : public b2World {
public:
	Level() : b2World(b2Vec2_zero) {}

	Image* GetImageList();
	const Image* GetImageList() const;

	bool FromJSON(const rapidjson::Value &value, ResourceManager &resManager);

	b2Body *FindBody(const char *id);
	b2Joint *FindJoint(const char *id);
	Image *FindImage(const char *id);

	void Render() const;
	void Update(float time);
private:
	void AddImage(Image *image);
	b2Body *LoadBody(const rapidjson::Value &value);
	b2Joint *LoadJoint(const rapidjson::Value &value);
	Image *LoadImage(const rapidjson::Value &value, ResourceManager &resManager);

	Camera camera;
	float textureScale;
	Image *images;
};

inline Image *Level::GetImageList() {
	return images;
}
inline const Image *Level::GetImageList() const {
	return images;
}
