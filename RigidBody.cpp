/*
 * RigidBody.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include <math.h>
#include <iostream>
#include "RigidBody.hpp"

#include "EventHandler.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Planet.hpp"

RigidBody::RigidBody() {
}

void RigidBody::initialize(GameObject* parent, float impulse, bool hasDrag, bool isOrbiting) {
	float angle = parent->transform_->rotation_.z;
	velocity_ = glm::vec3(cos(angle), sin(angle), 0.0f) * impulse;
	angularMomentum_ = 0.0f;
	drag_ = isOrbiting == false ? 0.995f : 1.0f;
	angularDrag_ = 0.9f;
	mass_ = 1.0f;

	if(isOrbiting)
	{
		float distance = glm::length(parent->transform_->position_);
		glm::vec3 direction = parent->transform_->position_ / distance;
		velocity_ = glm::vec3(direction.y, -direction.x, 0.0f) * sqrt(0.5f / distance);
	}
}

void RigidBody::update(Transform* transform) {
	/*if(controller != nullptr) {
		if(controller->isActing(Actions::STEER_RIGHT) == true)
			angularMomentum_ += 0.002f;
		else if(controller->isActing(Actions::STEER_LEFT) == true)
			angularMomentum_ -= 0.002f;
		float angle = transform->rotation_.z;
		if(controller->isActing(Actions::THRUST) == true)
			velocity_ += glm::vec3(cos(angle), sin(angle), 0.0f) * 0.01f;
	}*/
	transform->position_ += velocity_;
	transform->rotation_.z += angularMomentum_;
	velocity_ *= drag_;
	angularMomentum_ *= angularDrag_;
}

void RigidBody::updateGravity(GameObject* parent) {
	for(auto planet = GameObject::mainScene_->planets_.getStart(); planet != GameObject::mainScene_->planets_.getEnd(); ++planet)
	{
		if(planet->isValid_ && planet != parent)
		{
			glm::vec3 direction = planet->transform_->position_ - parent->transform_->position_;
			float distance = glm::length(direction);
			//velocity_ += (direction * mass_ * planet->GetRigidBody()->mass_) / (distance * distance * distance);
			velocity_ += (direction * mass_ * 0.5f) / (distance * distance * distance);
		}
	}
}

void RigidBody::Spin(float impulse) {
	angularMomentum_ += impulse;
}

void RigidBody::Drag(glm::vec3 impulse) {
	velocity_ += impulse;
}

RigidBody::~RigidBody() {
	// TODO Auto-generated destructor stub
}

