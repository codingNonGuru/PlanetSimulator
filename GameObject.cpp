/*
 * GameObject.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include <gtc/matrix_transform.hpp>

#include "GameObject.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "RigidBody.hpp"
#include "Controller.hpp"
#include "Scene.hpp"
#include "Collider.hpp"

Scene* GameObject::mainScene_ = nullptr;

void GameObject::initialize(bool isPlayer, Mesh* mesh, Transform* transform, float impulse, bool hasDrag, bool isOrbiting) {
	parent_ = nullptr;
	isValid_ = true;
	isWorking_ = true;
	transform_ = transform;

	rigidBody_ = new RigidBody();
	rigidBody_->initialize(this, impulse, hasDrag, isOrbiting);

	if(IsControlled())
	{
		if(isPlayer)
			controller_ = mainScene_->controllers_.allocate<HumanController>();
		else
			controller_ = mainScene_->controllers_.allocate<MachineController>();

		controller_->Initialize(this);
	}
	else
		controller_ = nullptr;

	mesh_ = mesh;
}

GameObject::GameObject() {
	// TODO Auto-generated constructor stub

}

GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}

void GameObject::UpdateCollisions()
{
	if(collider_)
		collider_->Update();
}

void GameObject::updatePhysics() {
	if(rigidBody_ != nullptr)
		rigidBody_->update(transform_);
}

void GameObject::updateGravity() {
	if(rigidBody_ != nullptr)
		rigidBody_->updateGravity(this);
}

void GameObject::draw(glm::mat4 &finalMatrix) {
	glm::mat4 worldMatrix = glm::translate(glm::mat4(1.0f), transform_->position_) * glm::rotate(glm::mat4(1.0f), transform_->rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	mesh_->draw(finalMatrix, worldMatrix);
}

void GameObject::Destroy()
{
	isValid_ = false;

	if(controller_)
	{
		mainScene_->controllers_.deallocate(controller_);
		controller_ = nullptr;
	}

	if(rigidBody_)
	{
		delete rigidBody_;
		rigidBody_ = nullptr;
	}

	if(transform_)
	{
		delete transform_;
		transform_ = nullptr;
	}

	if(collider_)
	{
		if(collider_->collision_)
		{
			mainScene_->collisions_.deallocate(collider_->collision_);
		}

		mainScene_->colliders_.deallocate(collider_);
		collider_ = nullptr;
	}
}

