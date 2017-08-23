#include "SDL2/SDL.h"

#include "Controller.hpp"
#include "EventHandler.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Transform.hpp"

bool Controller::isActing(Actions action) {
	return actions_[(int)action];
}

void Controller::SetAction(Actions action, bool value)
{
	actions_[(int)action] = value;
}

void Controller::Initialize(GameObject* parent)
{
	parent_ = parent;
}

void HumanController::update() {
	actions_[(int)Actions::STEER_LEFT] = EventHandler::isKeyPressed((int)SDLK_LEFT);
	actions_[(int)Actions::STEER_RIGHT] = EventHandler::isKeyPressed((int)SDLK_RIGHT);
	actions_[(int)Actions::THRUST] = EventHandler::isKeyPressed((int)SDLK_UP);
	actions_[(int)Actions::RETURN] = EventHandler::isKeyPressed((int)SDLK_DOWN);
	actions_[(int)Actions::MINE] = EventHandler::isKeyPressed((int)SDLK_m);
	actions_[(int)Actions::SHOOT] = EventHandler::isKeyPressed((int)SDLK_SPACE);
}

void MachineController::update() {
	Scene* scene = GameObject::mainScene_;
	Spaceship* otherShip = nullptr;
	float distance;
	for(Spaceship* ship = scene->ships_.getStart(); ship != scene->ships_.getEnd(); ++ship)
	{
		if(ship->isValid_ && ship != parent_)
			otherShip = ship;
	}

	glm::vec3 direction = otherShip->GetTransform()->position_ - parent_->GetTransform()->position_;
	float distance = glm::length(direction);
	direction /= distance;
	glm::vec3 forward = parent_->GetTransform()->GetForward();

	float dotProduct = glm::dot(direction, forward);
	glm::vec3 crossProduct = glm::cross(direction, forward);

	SetAction(Actions::STEER_LEFT, crossProduct.z > 0.0f);
	SetAction(Actions::STEER_RIGHT, crossProduct.z < 0.0f);
	SetAction(Actions::THRUST, distance > 3.0f && dotProduct > 0.7f);
	SetAction(Actions::RETURN, distance > 3.0f && dotProduct < -0.7f);
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
