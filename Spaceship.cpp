/*
 * Spaceship.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include <iostream>
#include <math.h>

#include "Spaceship.hpp"
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

Spaceship::Spaceship() {}

void Spaceship::updateLogic() {
	controller_->update();
	weapon_->update();

	Direction forward = transform_->GetForward();
	if(controller_->IsActing(Actions::SHOOT) && weapon_->CanFire()) {
		weapon_->Fire();
		float shootAngle = transform_->rotation_.z + utility::biasedRandom(-0.2f, 0.2f, 0.0f, 0.1f); // + utility::getRandom(-0.15f, 0.15f);
		Direction shootDirection(cos(shootAngle), sin(shootAngle), 0.0f);
		float speed = utility::getRandom(0.3f, 0.35f);
		auto projectile = mainScene_->projectiles_.allocate();
		Position position = transform_->position_ + shootDirection * utility::getRandom(0.7f, 1.0f);
		Rotation rotation = Rotation(0.0f, 0.0f, shootAngle);
		Transform* transform = new Transform(position, rotation, 1.0f);
		projectile->initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], transform, speed, false, false);
		projectile->SetParent(this);
	}
	if(controller_->IsActing(Actions::STEER_LEFT)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Spin(-0.001f);
	}
	if(controller_->IsActing(Actions::STEER_RIGHT)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Spin(0.001f);
	}
	if(controller_->IsActing(Actions::THRUST)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Drag(forward * 0.0015f);
	}
	if(controller_->IsActing(Actions::RETURN)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Drag(-forward * 0.0015f);
	}
	if(controller_->IsActing(Actions::COOL)) {
		weapon_->Cool();
	}

	if(controller_->isHuman()) {
		float leastDistance = 9999.9f;
		Asteroid* closestAsteroid = nullptr;
		for(auto asteroid = GameObject::mainScene_->asteroids_.getStart(); asteroid != GameObject::mainScene_->asteroids_.getEnd(); ++asteroid)
		{
			if(asteroid->isValid_)
			{
				auto direction = transform_->position_ - asteroid->transform_->position_;
				float distance = glm::length(direction);
				if(distance < leastDistance && distance < sensor_.GetLimit())
				{
					closestAsteroid = asteroid;
					leastDistance = distance;
				}
			}
		}
		sensor_.SetObject(closestAsteroid);
	}

	if(controller_->IsActing(Actions::MINE)) {
		GameObject* object = sensor_.GetObject();
		if(object != nullptr)
		{
			cargo_.AddOre(0.002f);
		}
	}

	//std::cout<<cargo_.GetOre()<<"\n";
}

Spaceship::~Spaceship() {
	// TODO Auto-generated destructor stub
}

void Spaceship::initialize(bool isPlayer, Mesh* mesh, Transform* transform, float impulse, bool hasDrag, bool isOrbiting) {
	GameObject::initialize(isPlayer, mesh, transform, impulse, hasDrag, false);

	weapon_ = mainScene_->weaponSystems_.allocate();
	weapon_->initialize(0.005f, 20.0f, 0.97f);

	collider_ = mainScene_->colliders_.allocate();
	collider_->Initialize(this, BoundingBoxes::MESH);

	sensor_ = Sensor();
	cargo_ = Cargo();
}

void Projectile::updateLogic() {
	lifeTime_ += 0.01f;
	bool hasCollided = collider_->collision_ != nullptr && collider_->collision_->collider_ != parent_;
	if(lifeTime_ > 2.0f || hasCollided)
	{
		isWorking_ = false;
		if(hasCollided)
		{
			Explosion* explosion = mainScene_->explosions_.allocate();
			Transform* transform = new Transform();
			*transform = *transform_;
			transform->position_ -= rigidBody_->velocity_ * 0.5f;
			explosion->initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], transform, 0.0f, false, false);
		}
	}
}

void Projectile::initialize(bool isPlayer, Mesh* mesh, Transform* transform, float impulse, bool hasDrag, bool isOrbiting) {
	GameObject::initialize(isPlayer, mesh, transform, impulse, hasDrag, isOrbiting);
	collider_ = mainScene_->colliders_.allocate();
	collider_->Initialize(this, BoundingBoxes::POINT);
	lifeTime_ = 0.0f;
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
