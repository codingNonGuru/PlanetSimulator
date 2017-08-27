/*
 * Scene.hpp
 *
 *  Created on: Oct 7, 2016
 *      Author: andrei
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "Memory.hpp"
#include "Spaceship.hpp"
#include "Asteroid.hpp"

class Controller;
class Planet;
class Spaceship;
class Collider;
class Collision;

class Scene {
public:
	container::DynamicPool<Spaceship, unsigned int> ships_;
	container::DynamicPool<Asteroid, unsigned int> asteroids_;
	container::DynamicPool<Planet, unsigned int> planets_;
	container::DynamicPool<Controller, unsigned int> controllers_;
	container::DynamicPool<Projectile, unsigned int> projectiles_;
	container::DynamicPool<Weapon, unsigned int> weaponSystems_;
	container::DynamicPool<Collider, unsigned int> colliders_;
	container::DynamicPool<Collision, unsigned int> collisions_;

	Spaceship* ownShip_;

	Scene();
	void initialize();
	virtual ~Scene();
	void UpdatePhysics();
	void UpdateCollisions();
};

#endif /* SCENE_HPP_ */
