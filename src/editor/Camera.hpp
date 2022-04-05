#pragma once
#include <b2_math.h>
#include "Colors.hpp"

class Camera{
public:
	Camera();

	void Apply(const Colors &colors, const wxSize &halfsize) const;

	void SetGrid(float grid);
	bool UpdateGrid(bool shift, bool ctrl);

	b2Vec2 ConvertToPixels(const b2Vec2 &p) const;
	b2Vec2 ConvertFromPixels(const b2Vec2 &p) const;
	b2Vec2 ToGrid(const b2Vec2 &p) const;
	float ToGrid(float p) const;
	float AngleToGrid(float p) const;

	void Drag(const b2Vec2 &mouse, const b2Vec2 &position);
	void Zoom(float value, const b2Vec2 &mouse, const b2Vec2 &position);

	b2Vec2 position;

	float gridSize;
	float currentGridSize;
	float currentAngleGrid;
	float zoom;
};

