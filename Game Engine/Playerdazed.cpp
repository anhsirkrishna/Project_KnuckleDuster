#include "Playerdazed.h"
#include "GameObject.h"
#include "GLSprite.h"
#include "InputManager.h"
#include "EventManager.h"

Playerdazed::Playerdazed() : Component("PLAYERDAZED"), p_owner_glsprite(NULL),
							 undazed_input_count(0), current_input_count(0), curr_input(0), prev_input(0) {}

void Playerdazed::Serialize(json json_object) {
	undazed_input_count = json_object["undazed_input_count"].get<int>();
}
void Playerdazed::Link() {
	std::string current_state = GetOwner()->CurrentState();
	GetOwner()->ChangeState("DAZED");
	p_owner_glsprite = static_cast<GLSprite*>(GetOwner()->HasComponent("GLSPRITE"));
	GetOwner()->ChangeState(current_state);
}

void Playerdazed::HandleInput() {
	curr_input = prev_input;
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_UP) || pInputManager->isKeyTriggered(SDL_SCANCODE_W)) {
		curr_input = SDL_SCANCODE_UP;
	}
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_DOWN) || pInputManager->isKeyTriggered(SDL_SCANCODE_S)) {
		curr_input = SDL_SCANCODE_DOWN;
	}
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_RIGHT) || pInputManager->isKeyTriggered(SDL_SCANCODE_D)) {
		curr_input = SDL_SCANCODE_RIGHT;
	}
	if (pInputManager->isKeyTriggered(SDL_SCANCODE_LEFT) || pInputManager->isKeyTriggered(SDL_SCANCODE_A)) {
		curr_input = SDL_SCANCODE_LEFT;
	}
}

void Playerdazed::Dazed() {
	GameObject* p_owner_obj = GetOwner();
	if (p_owner_obj->CurrentState() == "DOWNED") {
		p_owner_obj->ChangeState("DAZED");
	}
}

void Playerdazed::Update() {
	if (GetOwner()->CurrentState() == "DAZED") {
		if (curr_input != prev_input) {
			current_input_count++;
		}
		if (current_input_count >= undazed_input_count) {
			current_input_count = 0;
			undazed_input_count += 10;
			p_event_manager->QueueTimedEvent(
				new TimedEvent(EventID::revived, false, GetOwner()->index)
			);
		}
	}
	prev_input = curr_input;
}

void Playerdazed::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case (EventID::downed):
		Dazed();
		break;
	}
}
