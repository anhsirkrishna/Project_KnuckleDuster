#include "Hurting.h"
#include "Hurtbox.h"
#include "GLSprite.h"
#include "Animation.h"
#include "GameObject.h"
#include "Events.h"

Hurting::Hurting() : Component("HURTING"), p_owner_glsprite(NULL), p_owner_hurtbox(NULL), p_owner_animation(NULL) {}

void Hurting::Serialize(json json_object) {

}

void Hurting::Link() {
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
	std::string current_state = GetOwner()->CurrentState();
	GetOwner()->ChangeState("HURT");
	p_owner_glsprite = static_cast<GLSprite*>(GetOwner()->HasComponent("GLSPRITE"));
	p_owner_animation = static_cast<Animation*>(GetOwner()->HasComponent("ANIMATION"));
	GetOwner()->ChangeState(current_state);
}
void Hurting::Hurt() {
	GetOwner()->ChangeState("HURT");
	p_owner_animation->Refresh();
	SDL_Log("Got hit");
}

void Hurting::Update() {
	if (GetOwner()->CurrentState() == "HURT") {
		//Animation logic and state change
		if (p_owner_animation->Completed()) {
			GetOwner()->ChangeState("IDLE");
		}
	}
}

void Hurting::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::hit:
		Hurt();
		break;
	}
}