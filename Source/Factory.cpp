#include "Factory.hpp"
#include "Spaceship.hpp"
#include "Engine.hpp"
#include "Scene.hpp"
#include "Interface.hpp"
#include "RigidBody.hpp"
#include "HealthBar.hpp"

Scene* ShipFactory::scene_ = nullptr;
Interface* ShipFactory::interface_ = nullptr;

Spaceship* ShipFactory::Produce(bool isPlayer, ShipTypes shipType, Transform* transform)
{
	auto ship = scene_->ships_.allocate();

	if(shipType == ShipTypes::SCOUT)
	{
		RigidBody* rigidBody = new RigidBody(ship, 1.0f, 0.99f);
		Mesh* mesh = &Engine::meshes_[Meshes::SPACESHIP_SCOUT];

		ship->Initialize(isPlayer, mesh, transform, rigidBody);

		auto weapon = ship->GetWeapon();
		weapon->initialize(0.05f, 20.0f, 0.97f);

		auto hull = ship->GetHull();
		hull->Initialize(0.1f);
	}
	if(shipType == ShipTypes::CORVETTE)
	{
		RigidBody* rigidBody = new RigidBody(ship, 1.0f, 0.995f);
		Mesh* mesh = &Engine::meshes_[Meshes::SPACESHIP_CORVETTE];

		ship->Initialize(isPlayer, mesh, transform, rigidBody);

		auto weapon = ship->GetWeapon();
		weapon->initialize(0.001f, 20.0f, 0.97f);

		auto hull = ship->GetHull();
		hull->Initialize(1.0f);
	}

	auto healthBar = interface_->healthBars_.allocate();
	healthBar->Initialize(ship, &Engine::meshes_[Meshes::GENERIC_QUAD]);

	return ship;
}
