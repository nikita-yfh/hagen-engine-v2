#pragma once

#include "JSONUtils.hpp"
#include "box2d.h"

class Level : public b2World {
public:
	Level();

	bool FromJSON(const rapidjson::Value &value);

private:
	b2Body *LoadBody(const rapidjson::Value &value);
};

