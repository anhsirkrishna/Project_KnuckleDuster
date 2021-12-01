#include "Controller.h"
#include "GameObject.h"
#include "Walking.h"

Controller::Controller() : Component("CONTROLLER") { }

void Controller::HandleInput() {

	if (pInputManager->isKeyTriggered(SDL_SCANCODE_UP) || pInputManager->isKeyTriggered(SDL_SCANCODE_W)) {
		p_owner_walking->Walk(WalkDirection::Up);
	}
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_DOWN) || pInputManager->isKeyTriggered(SDL_SCANCODE_S)) {
		p_owner_walking->Walk(WalkDirection::Down);
	}
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_RIGHT) || pInputManager->isKeyTriggered(SDL_SCANCODE_D)) {
		p_owner_walking->Walk(WalkDirection::Right);
	}
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_LEFT) || pInputManager->isKeyTriggered(SDL_SCANCODE_A)) {
		p_owner_walking->Walk(WalkDirection::Left);
	}

	if (pInputManager->isKeyReleased(SDL_SCANCODE_UP) || pInputManager->isKeyReleased(SDL_SCANCODE_W)) {
		p_owner_walking->StopWalk(WalkDirection::Up);
	}
	if (pInputManager->isKeyReleased(SDL_SCANCODE_DOWN) || pInputManager->isKeyReleased(SDL_SCANCODE_S)) {
		p_owner_walking->StopWalk(WalkDirection::Down);
	}
	if (pInputManager->isKeyReleased(SDL_SCANCODE_RIGHT) || pInputManager->isKeyReleased(SDL_SCANCODE_D)) {
		p_owner_walking->StopWalk(WalkDirection::Right);
	}
	if (pInputManager->isKeyReleased(SDL_SCANCODE_LEFT) || pInputManager->isKeyReleased(SDL_SCANCODE_A)) {
		p_owner_walking->StopWalk(WalkDirection::Left);
	}

}


void Controller::Link() {
	p_owner_walking = static_cast<Walking*>(GetOwner()->HasComponent("WALKING"));
}

void Controller::Serialize(json json_object) {
}