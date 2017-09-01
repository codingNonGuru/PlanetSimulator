#include "Engine.hpp"
#include "SDL2/SDL.h"
#include "EventHandler.hpp"

Mouse EventHandler::mouse_ = Mouse();
int EventHandler::keyCount_ = 0;
container::Array<int> EventHandler::currentKeys_ = container::Array<int>();
container::Array<int> EventHandler::formerKeys_ = container::Array<int>();

void EventHandler::Initialize() {
	currentKeys_.initialize(64);
	formerKeys_.initialize(64);
}

void EventHandler::receiveInput() {
	formerKeys_.reset();
	for(auto key = currentKeys_.getStart(); key != currentKeys_.getEnd(); ++key)
		*formerKeys_.allocate() = *key;
	currentKeys_.reset();

	mouse_.scrollUp_ = false;
	mouse_.scrollDown_ = false;
	mouse_.lastLeft_ = mouse_.currentLeft_;
	mouse_.lastRight_ = mouse_.currentRight_;
	mouse_.currentRight_ = false;
	mouse_.currentLeft_ = false;
	int x, y;
	unsigned int mouseFlags = SDL_GetMouseState(&x, &y);
	mouse_.position_.x = x;
	mouse_.position_.y = y;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			Engine::ShutDown();
		}
		else {
			if(event.type == SDL_MOUSEBUTTONDOWN) {
				if(event.button.button == SDL_BUTTON_LEFT)
					mouse_.currentLeft_ = true;
				else if(event.button.button == SDL_BUTTON_RIGHT)
					mouse_.currentRight_ = true;
			}
			else if(event.type == SDL_MOUSEWHEEL) {
				if(event.wheel.y < 0)
					mouse_.scrollDown_ = true;
				else if(event.wheel.y > 0)
					mouse_.scrollUp_ = true;
			}
		}
	}
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	if(keyStates[SDL_SCANCODE_UP])
		*currentKeys_.allocate() = SDLK_UP;
	if(keyStates[SDL_SCANCODE_DOWN])
		*currentKeys_.allocate() = SDLK_DOWN;
	if(keyStates[SDL_SCANCODE_LEFT])
		*currentKeys_.allocate() = SDLK_LEFT;
	if(keyStates[SDL_SCANCODE_RIGHT])
		*currentKeys_.allocate() = SDLK_RIGHT;
	if(keyStates[SDL_SCANCODE_ESCAPE])
		*currentKeys_.allocate() = SDLK_ESCAPE;
	if(keyStates[SDL_SCANCODE_M])
		*currentKeys_.allocate() = SDLK_m;
	if(keyStates[SDL_SCANCODE_SPACE])
		*currentKeys_.allocate() = SDLK_SPACE;
	if(keyStates[SDL_SCANCODE_C]) //Center screen
		*currentKeys_.allocate() = SDLK_c;
	if(keyStates[SDL_SCANCODE_T]) //Travel
		*currentKeys_.allocate() = SDLK_t;
	if(keyStates[SDL_SCANCODE_F]) //Fight
		*currentKeys_.allocate() = SDLK_f;
	if(keyStates[SDL_SCANCODE_1]) //Open menu
		*currentKeys_.allocate() = SDLK_1;
	if(keyStates[SDL_SCANCODE_2]) //Open menu
		*currentKeys_.allocate() = SDLK_2;
	if(keyStates[SDL_SCANCODE_3]) //Open menu
		*currentKeys_.allocate() = SDLK_3;
	if(keyStates[SDL_SCANCODE_4]) //Open menu
		*currentKeys_.allocate() = SDLK_4;
	SDL_PumpEvents();
}

void EventHandler::processInput() {

}

bool EventHandler::isKeyPressed(int32_t key) {
	for(auto currKey = currentKeys_.getStart(); currKey != currentKeys_.getEnd(); ++currKey)
		if(*currKey == key)
			return true;
	return false;
}

bool EventHandler::wasKeyPressed(int32_t key) {
	int i = 0;
	for(auto formKey = formerKeys_.getStart(); formKey != formerKeys_.getEnd(); ++formKey, ++i) {
		if(*formKey == key && *formKey == *(currentKeys_.getStart() + i))
			return true;
		else if(*formKey == key)
			return false;
	}
	return false;
}

void EventHandler::update() {
	receiveInput();
	processInput();
}

