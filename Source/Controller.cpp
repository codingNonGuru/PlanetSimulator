#include <iostream>

#include "SDL2/SDL.h"

#include "Controller.hpp"
#include "EventHandler.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Transform.hpp"

bool Controller::IsActing(Actions action) {
	return actions_[(int)action];
}

void Controller::SetAction(Actions action, bool value)
{
	actions_[(int)action] = value;
}

void Controller::Initialize(Spaceship* parent)
{
	parent_ = parent;
	target_ = nullptr;
}

void HumanController::update() {
	actions_[(int)Actions::STEER_LEFT] = EventHandler::isKeyPressed((int)SDLK_LEFT);
	actions_[(int)Actions::STEER_RIGHT] = EventHandler::isKeyPressed((int)SDLK_RIGHT);
	actions_[(int)Actions::THRUST] = EventHandler::isKeyPressed((int)SDLK_UP);
	actions_[(int)Actions::RETURN] = EventHandler::isKeyPressed((int)SDLK_DOWN);
	actions_[(int)Actions::MINE] = EventHandler::isKeyPressed((int)SDLK_m);
	actions_[(int)Actions::SHOOT] = EventHandler::isKeyPressed((int)SDLK_SPACE);

	auto sensor = parent_->GetSensor();
	if(sensor)
		sensor->GetClosestAsteroid();
}

void MachineController::update() {
	Scene* scene = GameObject::mainScene_;
	Spaceship* otherShip = nullptr;
	float distance;
	for(Spaceship* ship = scene->ships_.getStart(); ship != scene->ships_.getEnd(); ++ship)
	{
		if(ship->isValid_ && ship->isWorking_ && ship != parent_)
			otherShip = ship;
	}

	for(int i = 0; i < (int)Actions::COUNT; ++i)
		actions_[i] = false;

	if(!otherShip)
	{
		return;
	}

	glm::vec3 direction = otherShip->GetTransform()->position_ - parent_->GetTransform()->position_;
	distance = glm::length(direction);
	direction /= distance;
	glm::vec3 forward = parent_->GetTransform()->GetForward();

	float dotProduct = glm::dot(direction, forward);
	glm::vec3 crossProduct = glm::cross(direction, forward);

	SetAction(Actions::STEER_LEFT, crossProduct.z > 0.0f);
	SetAction(Actions::STEER_RIGHT, crossProduct.z < 0.0f);
	SetAction(Actions::THRUST, (distance > 14.0f && dotProduct > 0.7f) || (distance < 8.0f && dotProduct < -0.7f));
	SetAction(Actions::RETURN, (distance > 14.0f && dotProduct < -0.7f) || (distance < 8.0f && dotProduct > 0.7f));
	SetAction(Actions::SHOOT, distance < 20.0f && dotProduct > 0.95f);
	SetAction(Actions::COOL, parent_->GetWeapon()->GetHeatFactor() > 0.95f);
}

void BargeController::update()
{
	if(!target_)
	{
		auto sensor = parent_->GetSensor();

		GameObject* asteroid = nullptr;
		if(sensor)
			asteroid = sensor->GetClosestAsteroid();

		target_ = asteroid;
	}

	for(int i = 0; i < (int)Actions::COUNT; ++i)
		actions_[i] = false;

	if(target_)
	{
		glm::vec3 direction = target_->GetTransform()->position_ - parent_->GetTransform()->position_;
		float distance = glm::length(direction);
		direction /= distance;
		glm::vec3 forward = parent_->GetTransform()->GetForward();

		float dotProduct = glm::dot(direction, forward);
		glm::vec3 crossProduct = glm::cross(direction, forward);

		SetAction(Actions::STEER_LEFT, crossProduct.z > 0.0f);
		SetAction(Actions::STEER_RIGHT, crossProduct.z < 0.0f);
		SetAction(Actions::THRUST, (distance > 8.0f && dotProduct > 0.7f) || (distance < 4.0f && dotProduct < -0.7f));
		SetAction(Actions::RETURN, (distance > 8.0f && dotProduct < -0.7f) || (distance < 4.0f && dotProduct > 0.7f));
		SetAction(Actions::MINE, distance < 8.0f);
	}
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
