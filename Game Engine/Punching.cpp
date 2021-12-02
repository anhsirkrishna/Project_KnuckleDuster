#include "Punching.h"
#include "GameObject.h"
#include "Hurtbox.h"
#include "GLSprite.h"
#include "Animation.h"

Punching::Punching() : Component("PUNCHING"), p_owner_hurtbox(NULL), p_owner_glsprite(NULL), p_owner_animation(NULL), sprite_index(0) {}

void Punching::Serialize(json json_object) {
}
void Punching::Link() {
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
	std::string current_state = GetOwner()->CurrentState();
	GetOwner()->ChangeState("PUNCH");
	p_owner_glsprite = static_cast<GLSprite*>(GetOwner()->HasComponent("GLSPRITE"));
	p_owner_animation = static_cast<Animation*>(GetOwner()->HasComponent("ANIMATION"));
	GetOwner()->ChangeState(current_state);
}

void Punching::Punch() {
	GetOwner()->ChangeState("PUNCH");
	p_owner_animation->Refresh();
}

void Punching::Update() {
	if (GetOwner()->CurrentState() == "PUNCH") {
		if (p_owner_animation->Completed()) {
			sprite_index = (sprite_index + 1) % 2;
			p_owner_glsprite->SetTexture(sprite_index);
			GetOwner()->ChangeState("IDLE");
		}
	}
}


