/*
 * Transform.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include <math.h>

#include "Transform.hpp"

Transform::Transform(glm::vec3 position, glm::vec3 rotation) {
	position_ = position;
	rotation_ = rotation;
}

glm::vec3 Transform::GetForward()
{
	return glm::vec3(cos(rotation_.z), sin(rotation_.z), 0.0f);
}

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

