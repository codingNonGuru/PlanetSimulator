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
class Transform;
class ParticleManager;

template <class Type>
using Array = container::Array<Type>;
template <class Type>
using Pool = container::DynamicPool<Type>;

class Scene {
public:
	Array <Asteroid> asteroids_;
	Array <Planet> planets_;

	Pool <Ship> ships_;
	Pool <Shell> shells_;
	Pool <Weapon> weaponSystems_;
	Pool <Explosion> explosions_;
	Pool <Structure> structures_;

	Pool <Transform> transforms_;
	Pool <Controller> controllers_;
	Pool <RigidBody> rigidBodies_;
	Pool <Collider> colliders_;

	Ship* ownShip_;

	Scene();
	void Initialize();
	virtual ~Scene();
	void UpdatePhysics();
	void UpdateCollisions();
	void UpdateLogic();
};

#endif /* SCENE_HPP_ */
