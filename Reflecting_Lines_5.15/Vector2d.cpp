#include "Vector2d.h"

Vector2d::Vector2d() {
	x = 0;
	y = 0;
}

void Vector2d::normalize() {
	float inv_mag = 1.0 / sqrt(x * x + y * y);
	x *= inv_mag;
	y *= inv_mag;
	isNormalized = true;
}

Vector2d:: Vector2d(float a, float b, bool normalize) {
	x = a, y = b;
	if (normalize)
		this->normalize();

};

void Vector2d::rotate(float theta) {
	float newX = (x * cos(theta) - y * sin(theta));
	y = (x * sin(theta) + y * cos(theta));
	x = newX;
}
