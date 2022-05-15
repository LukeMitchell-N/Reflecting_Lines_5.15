#pragma once
#include <cmath>
#
class Vector2d {
public:
	float x, y;
	bool isNormalized = false;

	void rotate(float theta);
	void normalize();
	Vector2d(float a, float b, bool normalize);
	Vector2d();
};

