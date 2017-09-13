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

float RigidBody::gravityConstant_ = 0.002f;

RigidBody::RigidBody() {
}

RigidBody::RigidBody(GameObject* parent, float mass, float drag)
{
	Initialize(parent, mass, drag);
}

void RigidBody::Initialize(GameObject* parent, float mass, float drag)
{
	parent_ = parent;
	angularMomentum_ = 0.0f;
	drag_ = drag;
	angularDrag_ = 0.95f;
	mass_ = mass;
}

void RigidBody::update(Transform* transform) {
	transform->position_ += velocity_;
	transform->rotation_.z += angularMomentum_;
	if(transform->rotation_.z > 6.2831f)
		transform->rotation_.z -= 6.2831f;
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
			velocity_ += (direction * mass_ * gravityConstant_) / (distance * distance * distance);
		}
	}
}

void RigidBody::Spin(float impulse)
{
	angularMomentum_ += impulse / mass_;
}

void RigidBody::Push(Direction impulse)
{
	velocity_ += impulse / mass_;
}

void RigidBody::PushForward(float impulse)
{
	velocity_ += parent_->GetTransform()->GetForward() * impulse / mass_;
}

void RigidBody::AddOrbitalVelocity(GameObject* attractor)
{
	if(attractor == nullptr)
		return;

	Direction direction = parent_->transform_->position_ - attractor->GetTransform()->position_;
	float distance = glm::length(direction);
	Direction orbitalVelocity = direction / distance;
	velocity_ += glm::vec3(orbitalVelocity.y, -orbitalVelocity.x, 0.0f) * sqrt(gravityConstant_ * mass_ / distance);
}

RigidBody::~RigidBody() {
	// TODO Auto-generated destructor stub
}

