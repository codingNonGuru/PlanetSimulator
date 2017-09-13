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

void GameObject::Initialize(bool isPlayer, Mesh* mesh, Transform* transform, RigidBody* rigidBody, Controller* controller) {
	parent_ = nullptr;
	isValid_ = true;
	isWorking_ = true;
	isAttached_ = true;

	mesh_ = mesh;
	transform_ = transform;
	rigidBody_ = rigidBody;
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

void GameObject::Draw(Matrix &finalMatrix) {
	Matrix worldMatrix = glm::translate(Matrix(1.0f), GetWorldPosition()) * glm::rotate(Matrix(1.0f), GetWorldRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(Matrix(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	mesh_->draw(finalMatrix, worldMatrix);

	OnDraw(finalMatrix, worldMatrix);
}

void GameObject::Destroy()
{
	isValid_ = false;
	parent_ = nullptr;

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
		mainScene_->colliders_.deallocate(collider_);
		collider_ = nullptr;
	}
}

Position GameObject::GetWorldPosition()
{
	if(!isAttached_)
		return transform_->position_;

	Position position = transform_->position_;
	GameObject* parent = parent_;
	while(true)
	{
		if(parent)
		{
			float rotation = parent->transform_->rotation_.z;
			float s = sin(rotation);
			float c = cos(rotation);

			float x = position.x * c - position.y * s;
			float y = position.x * s + position.y * c;

			position.x = parent->transform_->position_.x + x;
			position.y = parent->transform_->position_.y + y;

			parent = parent->parent_;
		}
		else
		{
			break;
		}
	}

	return position;
}

Rotation GameObject::GetWorldRotation()
{
	if(!isAttached_)
		return transform_->rotation_;

	Rotation rotation = transform_->rotation_;
	GameObject* parent = parent_;
	while(true)
	{
		if(parent)
		{
			rotation += parent->transform_->rotation_;

			parent = parent->parent_;
		}
		else
		{
			break;
		}
	}

	return rotation;
}

