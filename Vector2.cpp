#include "Vector2.hpp"
#include <math.h>

Vector2& Vector2::operator += (Vector2& other) {
	this->x_ += other.x_;
	this->y_ += other.y_;
}

Vector2& Vector2::operator *= (float factor) {
	this->x_ *= factor;
	this->y_ *= factor;
}

Vector2 Vector2::operator - (Vector2& other) {
	Vector2 result(this->x_ - other.x_, this->y_ - other.y_);
	return result;
}

Vector2 Vector2::operator * (float other) {
	Vector2 result(this->x_ * other, this->y_ * other);
	return result;
}

float Vector2::normalize() {
	float length = (x_ * x_) + (y_ * y_);
	length = sqrt(length);
	x_ /= length;
	y_ /= length;
	return length;
}

void Vector2::clear() {
	x_ = 0.0f;
	y_ = 0.0f;
}
