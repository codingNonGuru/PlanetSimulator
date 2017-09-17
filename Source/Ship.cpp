/*
 * Spaceship.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include "Ship.hpp"

#include <string.h>
#include <iostream>
#include <math.h>

#include "Engine.hpp"
#include "EventHandler.hpp"
#include "Controller.hpp"
#include "Scene.hpp"
#include "Mesh.hpp"
#include "RigidBody.hpp"
#include "Transform.hpp"
#include "Utility.hpp"
#include "Collider.hpp"
#include "Explosion.hpp"
#include "HealthBar.hpp"

Ship::Ship() {}

void Ship::UpdateLogic() {
	if(!isValid_ || !isWorking_)
		return;

	if(hull_.GetDamage() <= 0.0f)
	{
		isWorking_ = false;
		return;
	}

	controller_->update();
	weapon_->update();

	if(controller_->IsActing(Actions::SHOOT) && weapon_->CanFire()) {
		weapon_->Fire();
		{
			auto shell = mainScene_->shells_.allocate();

			float shootAngle = transform_->rotation_.z + utility::biasedRandom(-0.1f, 0.1f, 0.0f, 0.03f);
			Direction shootDirection(cos(shootAngle), sin(shootAngle), 0.0f);
			float speed = utility::getRandom(0.3f, 0.32f);

			Position position = transform_->position_ + shootDirection * utility::getRandom(0.2f, 0.3f);
			Rotation rotation = Rotation(0.0f, 0.0f, shootAngle);
			Transform* transform = new Transform(position, rotation, 0.4f);
			RigidBody* shellRigidBody = RigidBody::Allocate(mainScene_, shell, 1.0f, 0.995f);

			shell->Initialize(false, &Engine::meshes_[Meshes::SHELL], transform, shellRigidBody, nullptr);
			shellRigidBody->PushForward(speed);
			shell->SetParent(this);

			shell->isAttached_ = false;
		}
	}
	if(controller_->IsActing(Actions::STEER_LEFT)) {
		if(rigidBody_)
			rigidBody_->Spin(-thruster_.power_ * 0.75f);
	}
	if(controller_->IsActing(Actions::STEER_RIGHT)) {
		if(rigidBody_)
			rigidBody_->Spin(thruster_.power_ * 0.75f);
	}
	if(controller_->IsActing(Actions::THRUST)) {
		if(rigidBody_)
			rigidBody_->PushForward(thruster_.power_);
	}
	if(controller_->IsActing(Actions::RETURN)) {
		if(rigidBody_)
			rigidBody_->PushForward(-thruster_.power_);
	}
	if(controller_->IsActing(Actions::COOL)) {
		weapon_->Cool();
	}

	if(controller_->IsActing(Actions::MINE)) {
		GameObject* object = sensor_.GetObject();
		if(object != nullptr)
		{
			cargo_.AddOre(0.002f);
		}
	}
	if(controller_->IsActing(Actions::UNLOAD))
	{
		if(home_)
		{
			cargo_.RemoveOre(0.002f);
		}
	}
}

GameObject* Sensor::GetClosestAsteroid()
{
	float leastDistance = 9999.9f;
	Asteroid* closestAsteroid = nullptr;
	for(auto asteroid = GameObject::mainScene_->asteroids_.getStart(); asteroid != GameObject::mainScene_->asteroids_.getEnd(); ++asteroid)
	{
		if(asteroid->isValid_)
		{
			auto direction = parent_->transform_->position_ - asteroid->transform_->position_;
			float distance = glm::length(direction);
			if(distance < leastDistance && distance < GetLimit() * 10.0f)
			{
				closestAsteroid = asteroid;
				leastDistance = distance;
			}
		}
	}

	SetObject(closestAsteroid);

	return closestAsteroid;
}

GameObject* Sensor::GetRandomAsteroid()
{
	Asteroid* closestAsteroid = nullptr;
	for(auto asteroid = GameObject::mainScene_->asteroids_.getStart(); asteroid != GameObject::mainScene_->asteroids_.getEnd(); ++asteroid)
	{
		if(asteroid->isValid_)
		{
			auto direction = parent_->transform_->position_ - asteroid->transform_->position_;
			float distance = glm::length(direction);
			if(distance < GetLimit() * 10.0f)
			{
				closestAsteroid = asteroid;
				if(utility::throwChance(0.02f))
				{
					break;
				}
			}
		}
	}

	SetObject(closestAsteroid);

	return closestAsteroid;
}

Ship::~Ship() {
	// TODO Auto-generated destructor stub
}

void Ship::Initialize(bool isPlayer, Mesh* mesh, Transform* transform, RigidBody* rigidBody, Controller* controller) {
	GameObject::Initialize(isPlayer, mesh, transform, rigidBody, controller);
	strcpy(name_, "spaceship");

	collider_ = mainScene_->colliders_.allocate();
	collider_->Initialize(this, BoundingBoxes::MESH);

	weapon_ = mainScene_->weaponSystems_.allocate();

	sensor_ = Sensor(this);

	cargo_.Initialize(1.0f);

	if(!controller)
	{
		if(IsControlled())
		{
			if(isPlayer)
				controller_ = mainScene_->controllers_.allocate<HumanController>();
			else
				controller_ = mainScene_->controllers_.allocate<MachineController>();
		}
		else
			controller_ = nullptr;
	}
	else
	{
		controller_ = controller;
	}

	if(controller_)
		controller_->Initialize(this);
}

void Ship::OnDraw(Matrix& finalMatrix, Matrix& worldMatrix)
{
	//healthBar_->GetMesh()->draw(finalMatrix, worldMatrix);
}

void Ship::Collide(Collision* collision)
{
	bool isCollisionValid = collision->collider_->parent_ != this;
	if(isCollisionValid)
	{
		hull_.Damage(nullptr);
	}
}

bool Ship::IsIdle()
{
	if(controller_)
	{
		return controller_->IsIdle();
	}
	else
		return false;
}

void Ship::AssignMission(Missions mission)
{
	if(controller_)
		controller_->SetMission(mission, true);
}

void Shell::UpdateLogic() {
	if(!isValid_ || !isWorking_)
		return;

	lifeTime_ += 0.01f;
	if(lifeTime_ > 3.0f)
	{
		isWorking_ = false;
	}
}

void Shell::Initialize(bool isPlayer, Mesh* mesh, Transform* transform, RigidBody* rigidBody, Controller* controller)
{
	GameObject::Initialize(isPlayer, mesh, transform, rigidBody, controller);
	strcpy(name_, "shell");
	collider_ = mainScene_->colliders_.allocate();
	collider_->Initialize(this, BoundingBoxes::POINT);
	lifeTime_ = 0.0f;
}

void Shell::Collide(Collision* collision)
{
	bool isCollisionValid = collision->collider_ != parent_;
	if(isCollisionValid)
	{
		isWorking_ = false;
		Explosion* explosion = mainScene_->explosions_.allocate();
		Transform* transform = new Transform();
		*transform = *transform_;
		transform->position_ -= rigidBody_->velocity_ * 0.5f;
		transform->scale_ = 2.0f;
		explosion->Initialize(false, &Engine::meshes_[Meshes::QUAD], transform, nullptr, nullptr);
	}
}

void Weapon::initialize(float fireSpeed, Temperature overheatPoint, float coolingRate) {
	lastFire_ = 0.0f;
	fireSpeed_ = fireSpeed;
	temperature_ = 0.0f;
	overheatPoint_ = overheatPoint;
	coolingRate_ = coolingRate;
}

void Weapon::update() {
	lastFire_ += 0.01f;
	temperature_ *= coolingRate_;

	if(GetHeatFactor() < 0.3f)
		isCooling_ = false;
}

bool Weapon::CanFire() {
	return lastFire_ > fireSpeed_ && !isCooling_ && temperature_ < overheatPoint_;
}

float Weapon::GetHeatFactor()
{
	return temperature_ / overheatPoint_;
}

void Weapon::Fire()
{
	lastFire_ = 0.0f;
	temperature_ += 1.0f;
}

void Weapon::Cool()
{
	isCooling_ = true;
}

void Hull::Damage(Shell* shell)
{
	currentIntegrity_ -= 0.01f;
}

void Cargo::Initialize(float capacity)
{
	capacity_ = capacity;
	mineralOre_ = 0.0f;
}

void Thruster::Initialize(float power, float fuel)
{
	power_ = power;
	fuel_ = fuel;
}
