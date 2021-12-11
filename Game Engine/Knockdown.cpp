#include "Knockdown.h"
#include "GLSprite.h"
#include "Animation.h"
#include "Hurtbox.h"
#include "EventManager.h"


Knockdown::Knockdown() : Component("KNOCKDOWN"), p_owner_animation(NULL), p_owner_sprite(NULL), p_owner_hurtbox(NULL), knockdown_hurtbox() {}

void Knockdown::Serialize(json json_object) {
}

void Knockdown::Link() {
	GameObject* p_game_obj = GetOwner();
	std::string current_state = p_game_obj->CurrentState();
	p_game_obj->ChangeState("KNOCKDOWN");
	p_owner_sprite = static_cast<GLSprite*>(p_game_obj->HasComponent("GLSPRITE"));
	p_owner_animation = static_cast<Animation*>(p_game_obj->HasComponent("ANIMATION"));
	p_owner_hurtbox = static_cast<Hurtbox*>(p_game_obj->HasComponent("HURTBOX"));
	p_game_obj->ChangeState(current_state);
}

void Knockdown::KnockedDown() {
	GameObject* p_game_obj = GetOwner();
	if (p_game_obj->CurrentState() != "KNOCKDOWN") {
		p_game_obj->ChangeState("KNOCKDOWN");
		p_owner_hurtbox->SetSolid(false);
		p_owner_animation->Refresh();
	}
}

void Knockdown::GetUp() {
	GameObject* p_game_obj = GetOwner();
	if (p_game_obj->CurrentState() == "DAZED" ||
		p_game_obj->CurrentState() == "KNOCKDOWN") {
		p_game_obj->ChangeState("IDLE");
		p_owner_hurtbox->SetSolid(true);
		p_owner_animation->Refresh();
	}
}

void Knockdown::Update() {
	if (GetOwner()->CurrentState() == "KNOCKDOWN") {
		SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
		curr_position.x = curr_position.x - (2 * p_owner_hurtbox->GetScaleX());
		p_owner_hurtbox->SetPosition(curr_position);
		if (p_owner_animation->Completed()) {
			GetOwner()->ChangeState("DOWNED");
			TimedEvent* downed_event = new DownedEvent(GetOwner()->index);
			downed_event->SetTime(1000);
			p_event_manager->QueueTimedEvent(downed_event);
		}
	}
}

void Knockdown::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::health_zero:
		KnockedDown();
		break;
	case EventID::revived:
		GetUp();
		break;
	}
}