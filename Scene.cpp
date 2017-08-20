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

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

void Scene::initialize() {
	ownShip_ = nullptr;

	ships_.initialize(32);
	planets_.initialize(32);
	asteroids_.initialize(128);
	controllers_.initialize(128);
	projectiles_.initialize(512);
	weaponSystems_.initialize(512);

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
}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

