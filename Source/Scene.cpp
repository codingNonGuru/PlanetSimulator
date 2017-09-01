/*
 * Scene.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: andrei
 */

#include "Scene.hpp"
#include "Controller.hpp"
#include "Planet.hpp"
#include "Spaceship.hpp"
#include "Collider.hpp"
#include "Explosion.hpp"

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

void Scene::initialize() {
	ownShip_ = nullptr;

	ships_.initialize(32);
	planets_.initialize(32);
	asteroids_.initialize(256);
	controllers_.initialize(128);
	projectiles_.initialize(1024);
	weaponSystems_.initialize(512);
	colliders_.initialize(2048);
	collisions_.initialize(1024);
	explosions_.initialize(256);

	for(Spaceship* ship = ships_.getStart(); ship != ships_.getEnd(); ++ship) {
		ship->isValid_ = false;
		ship->isWorking_ = false;
	}

	for(auto projectile = projectiles_.getStart(); projectile != projectiles_.getEnd(); ++projectile) {
		projectile->isValid_ = false;
		projectile->isWorking_ = false;
	}

	for(auto planet = planets_.getStart(); planet != planets_.getEnd(); ++planet) {
		planet->isValid_ = false;
	}

	for(auto asteroid = asteroids_.getStart(); asteroid != asteroids_.getEnd(); ++asteroid) {
		asteroid->isValid_ = false;
	}

	for(Explosion* explosion = explosions_.getStart(); explosion != explosions_.getEnd(); ++explosion)
	{
		explosion->isValid_ = false;
	}
}

void Scene::UpdateCollisions()
{
	for(Projectile* projectile = projectiles_.getStart(); projectile != projectiles_.getEnd(); ++projectile)
		if(projectile->isValid_ && projectile->isWorking_)
			projectile->UpdateCollisions();
	for(Spaceship* ship = ships_.getStart(); ship != ships_.getEnd(); ++ship)
		if(ship->isValid_ && ship->isWorking_)
			ship->UpdateCollisions();
}

void Scene::UpdatePhysics()
{
	std::cout<<explosions_.getSize()<<"\n";

	for(auto asteroid = asteroids_.getStart(); asteroid != asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
			asteroid->updateGravity();

	for(Spaceship* ship = ships_.getStart(); ship != ships_.getEnd(); ++ship)
		if(ship->isValid_ && ship->isWorking_)
			ship->updatePhysics();
	for(Projectile* projectile = projectiles_.getStart(); projectile != projectiles_.getEnd(); ++projectile)
		if(projectile->isValid_ && projectile->isWorking_)
			projectile->updatePhysics();
	for(Planet* planet = planets_.getStart(); planet != planets_.getEnd(); ++planet)
		if(planet->isValid_)
			planet->updatePhysics();
	for(Asteroid* asteroid = asteroids_.getStart(); asteroid != asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
			asteroid->updatePhysics();
}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

