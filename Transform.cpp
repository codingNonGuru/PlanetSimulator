/*
 * Transform.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include "Transform.hpp"

Transform::Transform(glm::vec3 position, glm::vec3 rotation) {
	position_ = position;
	rotation_ = rotation;
}

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

