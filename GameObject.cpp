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

Scene* GameObject::mainScene_ = nullptr;

void GameObject::initialize(bool isPlayer, Mesh* mesh, glm::vec3 position, glm::vec3 rotation, float impulse, bool hasDrag, bool isOrbiting) {
	isValid_ = true;
	isWorking_ = true;
	transform_ = new Transform(position, rotation);
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
	mesh_ = mesh;
}

GameObject::GameObject() {
	// TODO Auto-generated constructor stub

}

GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
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

