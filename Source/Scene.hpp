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
class Explosion;

class Scene {
public:
	container::DynamicPool<Spaceship> ships_;
	container::DynamicPool<Asteroid> asteroids_;
	container::DynamicPool<Planet> planets_;
	container::DynamicPool<Controller> controllers_;
	container::DynamicPool<Shell> shells_;
	container::DynamicPool<Weapon> weaponSystems_;
	container::DynamicPool<Collider> colliders_;
	container::DynamicPool<Explosion> explosions_;

	Spaceship* ownShip_;

	Scene();
	void initialize();
	virtual ~Scene();
	void UpdatePhysics();
	void UpdateCollisions();
};

#endif /* SCENE_HPP_ */
