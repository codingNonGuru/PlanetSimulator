/*
 * Transform.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include <math.h>

#include "Transform.hpp"

Transform::Transform(Position position, Rotation rotation, Scale scale) {
	position_ = position;
	rotation_ = rotation;
	scale_ = scale;
}

Direction Transform::GetForward()
{
	return Direction(cos(rotation_.z), sin(rotation_.z), 0.0f);
}

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

