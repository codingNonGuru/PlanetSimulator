/*
 * Scene.hpp
 *
 *  Created on: Oct 7, 2016
 *      Author: andrei
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "Memory.hpp"
#include "Asteroid.hpp"
#include "Ship.hpp"

class Controller;
class Planet;
class Ship;
class Collider;
class RigidBody;
class Explosion;
class Structure;

class Scene {
public:
	container::DynamicPool<Ship> ships_;
	container::DynamicPool<Asteroid> asteroids_;
	container::DynamicPool<Planet> planets_;
	container::DynamicPool<Shell> shells_;
	container::DynamicPool<Weapon> weaponSystems_;
	container::DynamicPool<Explosion> explosions_;
	container::DynamicPool<Structure> structures_;

	container::DynamicPool<Controller> controllers_;
	container::DynamicPool<RigidBody> rigidBodies_;
	container::DynamicPool<Collider> colliders_;

	Ship* ownShip_;

	Scene();
	void Initialize();
	virtual ~Scene();
	void UpdatePhysics();
	void UpdateCollisions();
	void UpdateLogic();
};

#endif /* SCENE_HPP_ */
