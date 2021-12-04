#include "Controller.h"
#include "GameObject.h"
#include "Walking.h"
#include "Punching.h"
#include "Jumpkick.h"

Controller::Controller() : Component("CONTROLLER"), p_owner_walking(NULL), p_owner_punching(NULL) { }

void Controller::HandleInput() {
	if (GetOwner()->CurrentState() == "JUMPKICK" ||
		GetOwner()->CurrentState() == "PUNCH")
		return;
	if (pInputManager->isKeyPressed(SDL_SCANCODE_UP) || pInputManager->isKeyPressed(SDL_SCANCODE_W)) {
		p_owner_walking->Walk(WalkDirection::Up);
		input_buffer.push(SDL_SCANCODE_UP);
	}
	if (pInputManager->isKeyPressed(SDL_SCANCODE_DOWN) || pInputManager->isKeyPressed(SDL_SCANCODE_S)) {
		p_owner_walking->Walk(WalkDirection::Down);
		input_buffer.push(SDL_SCANCODE_DOWN);
	}
	if (pInputManager->isKeyPressed(SDL_SCANCODE_RIGHT) || pInputManager->isKeyPressed(SDL_SCANCODE_D)) {
		p_owner_walking->Walk(WalkDirection::Right);
		input_buffer.push(SDL_SCANCODE_RIGHT);
	}
	if (pInputManager->isKeyPressed(SDL_SCANCODE_LEFT) || pInputManager->isKeyPressed(SDL_SCANCODE_A)) {
		p_owner_walking->Walk(WalkDirection::Left);
		input_buffer.push(SDL_SCANCODE_LEFT);
	}

	if (pInputManager->isKeyReleased(SDL_SCANCODE_UP) || pInputManager->isKeyReleased(SDL_SCANCODE_W)) {
		p_owner_walking->StopWalk(WalkDirection::Up);
		input_buffer.push(SDL_SCANCODE_LEFT);
	}
	if (pInputManager->isKeyReleased(SDL_SCANCODE_DOWN) || pInputManager->isKeyReleased(SDL_SCANCODE_S)) {
		p_owner_walking->StopWalk(WalkDirection::Down);
		input_buffer.push(SDL_SCANCODE_LEFT);
	}
	if (pInputManager->isKeyReleased(SDL_SCANCODE_RIGHT) || pInputManager->isKeyReleased(SDL_SCANCODE_D)) {
		p_owner_walking->StopWalk(WalkDirection::Right);
		input_buffer.push(SDL_SCANCODE_LEFT);
	}
	if (pInputManager->isKeyReleased(SDL_SCANCODE_LEFT) || pInputManager->isKeyReleased(SDL_SCANCODE_A)) {
		p_owner_walking->StopWalk(WalkDirection::Left);
		input_buffer.push(SDL_SCANCODE_LEFT);
	}

	if (pInputManager->isKeyTriggered(SDL_SCANCODE_Z))
		p_owner_punching->Punch();
	
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_X))
		p_owner_jumpkick->Kick();
}


void Controller::Link() {
	p_owner_walking = static_cast<Walking*>(GetOwner()->HasComponent("WALKING"));
	p_owner_punching = static_cast<Punching*>(GetOwner()->HasComponent("PUNCHING"));
	p_owner_jumpkick = static_cast<Jumpkick*>(GetOwner()->HasComponent("JUMPKICK"));
}

void Controller::Serialize(json json_object) {
}