#include "SDL2/SDL.h"

#include "Controller.hpp"
#include "EventHandler.hpp"

bool Controller::isActing(Actions action) {
	return actions_[(int)action];
}

void HumanController::update() {
	actions_[(int)Actions::STEER_LEFT] = EventHandler::isKeyPressed((int)SDLK_LEFT);
	actions_[(int)Actions::STEER_RIGHT] = EventHandler::isKeyPressed((int)SDLK_RIGHT);
	actions_[(int)Actions::THRUST] = EventHandler::isKeyPressed((int)SDLK_UP);
	actions_[(int)Actions::MINE] = EventHandler::isKeyPressed((int)SDLK_m);
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
