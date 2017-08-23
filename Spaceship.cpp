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

Spaceship::Spaceship() {}

void Spaceship::updateLogic() {
	controller_->update();
	weapon_->update();

	glm::vec3 forward(cos(transform_->rotation_.z), sin(transform_->rotation_.z), 0.0f);
	if(controller_->isActing(Actions::SHOOT) == true && weapon_->canFire() == true) {
		weapon_->lastFire_ = 0.0f;
		auto projectile = mainScene_->projectiles_.allocate();
		projectile->initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], transform_->position_ + forward * 1.0f, transform_->rotation_, 0.5f, false, false);
	}
	if(controller_->isActing(Actions::STEER_LEFT)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Spin(-0.001f);
	}
	if(controller_->isActing(Actions::STEER_RIGHT)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Spin(0.001f);
	}
	if(controller_->isActing(Actions::THRUST)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Drag(forward * 0.001f);
	}
	if(controller_->isActing(Actions::RETURN)) {
		if(rigidBody_ != nullptr)
			rigidBody_->Drag(-forward * 0.001f);
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

	if(controller_->isActing(Actions::MINE)) {
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

void Spaceship::initialize(bool isPlayer, Mesh* mesh, glm::vec3 position, glm::vec3 rotation, float impulse, bool hasDrag, bool isOrbiting) {
	GameObject::initialize(isPlayer, mesh, position, rotation, impulse, hasDrag, false);
	weapon_ = mainScene_->weaponSystems_.allocate();
	weapon_->initialize();
	sensor_ = Sensor();
	cargo_ = Cargo();
}

void Projectile::updateLogic() {
	lifeTime_ += 0.01f;
	if(lifeTime_ > 1.0f)
		isWorking_ = false;
}

void Projectile::initialize(bool isPlayer, Mesh* mesh, glm::vec3 position, glm::vec3 rotation, float impulse, bool hasDrag, bool isOrbiting) {
	GameObject::initialize(isPlayer, mesh, position, rotation, impulse, hasDrag, false);
	lifeTime_ = 0.0f;
}

void Weapon::initialize() {
	lastFire_ = 0.0f;
	fireSpeed_ = 0.1f;
}

void Weapon::update() {
	lastFire_ += 0.01f;
}

bool Weapon::canFire() {
	return lastFire_ > fireSpeed_;
}
