/*
 * Scene.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: andrei
 */

#include "Scene.hpp"
#include "Controller.hpp"
#include "Planet.hpp"
#include "Collider.hpp"
#include "RigidBody.hpp"
#include "Explosion.hpp"
#include "Ship.hpp"
#include "Structure.hpp"

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

template<class ObjectType>
void Disable(ObjectType* start, ObjectType* end)
{
	for(auto object = start; object != end; ++object)
	{
		object->isValid_ = false;
		object->isWorking_ = false;
	}
}

template<class ObjectType>
void Execute(ObjectType* start, ObjectType* end, void(ObjectType::*function)())
{
	for(auto object = start; object != end; ++object)
	{
		(object->*function)();
	}
}

void Scene::Initialize() {
	ownShip_ = nullptr;

	ships_.initialize(32);
	planets_.initialize(32);
	asteroids_.initialize(256);
	controllers_.initialize(128);
	shells_.initialize(1024);
	weaponSystems_.initialize(512);
	colliders_.initialize(2048);
	explosions_.initialize(256);
	structures_.initialize(128);
	rigidBodies_.initialize(1024);

	Disable<Ship>(ships_.getStart(), ships_.getEnd());
	Disable<Shell>(shells_.getStart(), shells_.getEnd());
	Disable<Planet>(planets_.getStart(), planets_.getEnd());
	Disable<Asteroid>(asteroids_.getStart(), asteroids_.getEnd());
	Disable<Explosion>(explosions_.getStart(), explosions_.getEnd());
	Disable<Structure>(structures_.getStart(), structures_.getEnd());
}

void Scene::UpdateLogic()
{
	//std::cout<<rigidBodies_.getSize()<<"\n";

	Execute<Shell>(shells_.getStart(), shells_.getEnd(), &Shell::UpdateLogic);
	Execute<Ship>(ships_.getStart(), ships_.getEnd(), &Ship::UpdateLogic);
	Execute<Explosion>(explosions_.getStart(), explosions_.getEnd(), &Explosion::UpdateLogic);
	Execute<Structure>(structures_.getStart(), structures_.getEnd(), &Structure::UpdateLogic);
}

void Scene::UpdateCollisions()
{
	Execute<Shell>(shells_.getStart(), shells_.getEnd(), &Shell::UpdateCollisions);
	Execute<Ship>(ships_.getStart(), ships_.getEnd(), &Ship::UpdateCollisions);
}

void Scene::UpdatePhysics()
{
	for(auto asteroid = asteroids_.getStart(); asteroid != asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
			asteroid->updateGravity();

	for(Ship* ship = ships_.getStart(); ship != ships_.getEnd(); ++ship)
		if(ship->isValid_ && ship->isWorking_)
			ship->updatePhysics();
	for(Shell* projectile = shells_.getStart(); projectile != shells_.getEnd(); ++projectile)
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

