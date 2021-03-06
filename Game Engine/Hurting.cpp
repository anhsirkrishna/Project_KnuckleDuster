#include "Hurting.h"
#include "Hurtbox.h"
#include "GLSprite.h"
#include "Animation.h"
#include "GameObject.h"
#include "EventManager.h"

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
void Hurting::Hurt(int damage, int direction, int hit_distance) {
	GameObject* owner_object = GetOwner();
	if (owner_object->CurrentState() != "HURT" &&
		owner_object->CurrentState() != "KNOCKDOWN" &&
		owner_object->CurrentState() != "DOWNED") {
		p_owner_hurtbox->SetScale(
			direction*-1, 
			p_owner_hurtbox->GetScaleY());
		owner_object->ChangeState("HURT");
		p_owner_animation->Refresh();
		move_distance = hit_distance / p_owner_animation->Duration();
		p_event_manager->QueueTimedEvent(
			new TakeDamageEvent(damage, owner_object->index)
		);
	}
}

void Hurting::Update() {
	if (GetOwner()->CurrentState() == "HURT") {
		SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
		curr_position.x = curr_position.x + (move_distance * p_owner_hurtbox->GetScaleX() * -1);
		p_owner_hurtbox->SetPosition(curr_position);
		//Animation logic and state change
		if (p_owner_animation->Completed()) {
			GetOwner()->ChangeState("IDLE");
		}
	}
}

void Hurting::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::hit:
		Hurt(static_cast<HitEvent*>(p_event)->hit_damage, 
			 static_cast<HitEvent*>(p_event)->direction, 
			 static_cast<HitEvent*>(p_event)->hit_move_distance);
		break;
	}
}