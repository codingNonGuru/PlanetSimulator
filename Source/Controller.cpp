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

bool Controller::GetMission(Missions mission)
{
	return missions_[(int)mission];
}

void Controller::SetMission(Missions mission, bool value)
{
	missions_[(int)mission] = value;
}

bool Controller::IsIdle()
{
	for(int i = 0; i < (int)Missions::COUNT; ++i)
		if(missions_[i])
			return false;

	return true;
}

void Controller::Initialize(Ship* parent)
{
	parent_ = parent;
	target_ = nullptr;

	for(int i = 0; i < (int)Missions::COUNT; ++i)
		missions_[i] = false;
}

float distance;
float angleFactor;
Direction direction;

void Controller::ComputeDistance()
{
	direction = target_->GetWorldPosition() - parent_->GetTransform()->position_;
	distance = glm::length(direction);
}

void Controller::ReachTarget(float minimumDistance, float maximumDistance)
{
	direction /= distance;
	Direction forward = parent_->GetTransform()->GetForward();

	float angleFactor = glm::dot(direction, forward);
	glm::vec3 crossProduct = glm::cross(direction, forward);

	SetAction(Actions::STEER_LEFT, crossProduct.z > 0.0f);
	SetAction(Actions::STEER_RIGHT, crossProduct.z < 0.0f);
	SetAction(Actions::THRUST, (distance > maximumDistance && angleFactor > 0.7f) || (distance < minimumDistance && angleFactor < -0.7f));
	SetAction(Actions::RETURN, (distance > maximumDistance && angleFactor < -0.7f) || (distance < minimumDistance && angleFactor > 0.7f));
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
	Ship* otherShip = nullptr;
	for(Ship* ship = scene->ships_.getStart(); ship != scene->ships_.getEnd(); ++ship)
	{
		if(ship->isValid_ && ship->isWorking_ && ship != parent_)
			otherShip = ship;
	}

	for(int i = 0; i < (int)Actions::COUNT; ++i)
		actions_[i] = false;

	target_ = otherShip;
	if(target_)
	{
		ComputeDistance();
		ReachTarget(8.0f, 14.0f);

		SetAction(Actions::SHOOT, distance < 20.0f && angleFactor > 0.95f);
		SetAction(Actions::COOL, parent_->GetWeapon()->GetHeatFactor() > 0.95f);
	}
}

void BargeController::update()
{
	for(int i = 0; i < (int)Actions::COUNT; ++i)
		actions_[i] = false;

	float capacityFactor = parent_->cargo_.GetCapacityFactor();

	if(GetMission(Missions::MINE))
	{
		SetAction(Actions::DETACH, parent_->isAttached_);

		if(capacityFactor > 0.95f)
		{
			SetMission(Missions::MINE, false);
			SetMission(Missions::DELIVER, true);
			target_ = nullptr;
			return;
		}

		if(!target_)
		{
			auto sensor = parent_->GetSensor();

			GameObject* asteroid = nullptr;
			if(sensor)
				asteroid = sensor->GetRandomAsteroid();

			target_ = asteroid;
		}

		if(target_)
		{
			ComputeDistance();
			ReachTarget(4.0f, 8.0f);

			SetAction(Actions::MINE, distance < 8.0f);
		}
	}
	else if(GetMission(Missions::DELIVER))
	{
		if(capacityFactor < 0.05f)
		{
			SetMission(Missions::MINE, true);
			SetMission(Missions::DELIVER, false);
			target_ = nullptr;
			return;
		}

		target_ = parent_->origin_;
		if(target_)
		{
			ComputeDistance();
			ReachTarget(2.5f, 3.0f);

			SetAction(Actions::UNLOAD, distance < 4.0f);
		}
	}

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
