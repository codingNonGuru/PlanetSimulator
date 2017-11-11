#include "Factory.hpp"
#include "Engine.hpp"
#include "Scene.hpp"
#include "Interface.hpp"
#include "RigidBody.hpp"
#include "HealthBar.hpp"
#include "Controller.hpp"
#include "Ship.hpp"

Scene* ShipFactory::scene_ = nullptr;
Interface* ShipFactory::interface_ = nullptr;

Ship* ShipFactory::Produce(bool isPlayer, ShipTypes shipType, Transform* transform)
{
	auto ship = scene_->ships_.allocate();

	if(shipType == ShipTypes::SCOUT)
	{
		RigidBody* rigidBody = scene_->rigidBodies_.allocate();
		rigidBody->Initialize(ship, 1.0f, 0.99f);
		Mesh* mesh = &Engine::meshes_[Meshes::SHIP_SCOUT];

		ship->Initialize(isPlayer, mesh, transform, rigidBody, nullptr);

		auto weapon = ship->GetWeapon();
		weapon->initialize(0.05f, 20.0f, 0.97f);

		auto hull = ship->GetHull();
		hull->Initialize(0.1f);

		auto thruster = ship->GetThruster();
		thruster->Initialize(0.003f, 50.0f);
	}
	if(shipType == ShipTypes::CORVETTE)
	{
		auto rigidBody = RigidBody::Allocate(scene_, ship, 2.0f, 0.99f);
		Mesh* mesh = &Engine::meshes_[Meshes::SHIP_CORVETTE];

		ship->Initialize(isPlayer, mesh, transform, rigidBody, nullptr);

		auto weapon = ship->GetWeapon();
		weapon->initialize(0.05f, 20.0f, 0.9f);

		auto hull = ship->GetHull();
		hull->Initialize(1.0f);

		auto thruster = ship->GetThruster();
		thruster->Initialize(0.002f, 50.0f);
	}
	if(shipType == ShipTypes::BARGE)
	{
		RigidBody* rigidBody = scene_->rigidBodies_.allocate();
		rigidBody->Initialize(ship, 15.0f, 0.99f);
		Mesh* mesh = &Engine::meshes_[Meshes::SHIP_BARGE];
		Controller* controller = scene_->controllers_.allocate<BargeController>();

		ship->Initialize(isPlayer, mesh, transform, rigidBody, controller);

		auto weapon = ship->GetWeapon();
		weapon->initialize(0.05f, 20.0f, 0.97f);

		auto hull = ship->GetHull();
		hull->Initialize(2.0f);

		auto thruster = ship->GetThruster();
		thruster->Initialize(0.007f, 50.0f);
	}

	auto healthBar = interface_->healthBars_.allocate();
	healthBar->Initialize(ship, &Engine::meshes_[Meshes::QUAD]);

	return ship;
}
