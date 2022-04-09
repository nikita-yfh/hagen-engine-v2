#pragma once

#include "imgui.h"
#include "JSONUtils.hpp"

struct Style : public ImGuiStyle{
	bool FromJSON(const rapidjson::Value &value);

	void Apply() const;
};
