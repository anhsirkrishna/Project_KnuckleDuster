#include "Controller.h"
#include "GameObject.h"
#include "Hurtbox.h"

Controller::Controller() : Component("CONTROLLER"), p_owner_hurtbox(NULL) { }

void Controller::HandleInput() {
	SDL_Rect currPosition = p_owner_hurtbox->GetPosition();

	if (pInputManager->isKeyPressed(SDL_SCANCODE_UP) || pInputManager->isKeyPressed(SDL_SCANCODE_W)) {
		currPosition.y = currPosition.y - 5;
	}
	if (pInputManager->isKeyPressed(SDL_SCANCODE_DOWN) || pInputManager->isKeyPressed(SDL_SCANCODE_S)) {
		currPosition.y = currPosition.y + 5;
	}
	if (pInputManager->isKeyPressed(SDL_SCANCODE_RIGHT) || pInputManager->isKeyPressed(SDL_SCANCODE_D)) {
		currPosition.x = currPosition.x + 5;
	}
	if (pInputManager->isKeyPressed(SDL_SCANCODE_LEFT) || pInputManager->isKeyPressed(SDL_SCANCODE_A)) {
		currPosition.x = currPosition.x - 5;
	}

	p_owner_hurtbox->SetPosition(currPosition);
}


void Controller::Link() {
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
}

void Controller::Serialize(json json_object) {
}