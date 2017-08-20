#pragma once

class Vector2 {
public:
	float x_, y_;

	Vector2 () {}

	Vector2 (float x, float y) : x_(x), y_(y) {}

	Vector2& operator += (Vector2& other);

	Vector2& operator *= (float factor);

	Vector2 operator - (Vector2& other);

	Vector2 operator * (float other);

	float normalize();

	void clear();
};
