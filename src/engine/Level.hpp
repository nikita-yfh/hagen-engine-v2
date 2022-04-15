#pragma once

#include "ResourceManager.hpp"
#include "Image.hpp"
#include "Camera.hpp"
#include "JSONUtils.hpp"
#include "glad.h"
#include "box2d.h"

class Level : public b2World {
public:
	Level(ResourceManager &resManager);

	Image* GetImageList();
	const Image* GetImageList() const;

	bool FromJSON(const rapidjson::Value &value);

	b2Body *FindBody(const char *id);
	b2Joint *FindJoint(const char *id);
	Image *FindImage(const char *id);

	void Render() const;
private:
	b2Body *LoadBody(const rapidjson::Value &value);
	b2Joint *LoadJoint(const rapidjson::Value &value);
	Image *LoadImage(const rapidjson::Value &value);

	Camera camera;
	float textureScale;
	Image *images;
	ResourceManager &resManager;
};

inline Image *Level::GetImageList() {
	return images;
}
inline const Image *Level::GetImageList() const {
	return images;
}
