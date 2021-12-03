#include "Punching.h"
#include "GameObject.h"
#include "Hurtbox.h"
#include "GLSprite.h"
#include "Animation.h"
#include "GameObjectManager.h"
#include "Collision.h"
#include "EventManager.h"


Punching::Punching() : Component("PUNCHING"), p_owner_hurtbox(NULL), p_owner_glsprite(NULL), p_owner_animation(NULL), sprite_index(0), 
					   hitbox(), punch_damage(0), hitbox_x_offset(0), hitbox_y_offset(0), prev_state("IDLE") {}

void Punching::Serialize(json json_object) {
	auto hitbox_coords = json_object["hitbox"].get<std::vector<int>>();
	hitbox_x_offset = hitbox_coords[0];
	hitbox_y_offset = hitbox_coords[1];
	hitbox.w = hitbox_coords[2];
	hitbox.h = hitbox_coords[3];

	punch_damage = json_object["punch_damage"].get<int>();
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
	GameObject* game_obj = GetOwner();
	prev_state = game_obj->CurrentState();
	if (prev_state != "PUNCH") {
		GetOwner()->ChangeState("PUNCH");
		UpdateHitbox();
		p_owner_animation->Refresh();
	}
}

void Punching::Update() {
	if (GetOwner()->CurrentState() == "PUNCH") {
		//Collision logic
		CheckCollision();

		//Animation logic and state change
		if (p_owner_animation->Completed()) {
			sprite_index = (sprite_index + 1) % 2;
			p_owner_glsprite->SetTexture(sprite_index);
			GetOwner()->ChangeState(prev_state);
		}
	}
}

void Punching::UpdateHitbox() {
	hitbox.x = p_owner_hurtbox->GetPosition().x + (hitbox_x_offset * p_owner_hurtbox->GetScaleX());
	hitbox.y = p_owner_hurtbox->GetPosition().y + (hitbox_y_offset * p_owner_hurtbox->GetScaleY());
}

void Punching::CheckCollision() {
	//Go through all other hurtboxes in the game
	GameObject* curr_obj;
	Hurtbox* obj_hurtbox;
	for (unsigned int i = 0; i < GetOwner()->index; i++) {
		curr_obj = pGameObjectManager->game_object_list[i];
		if (curr_obj == NULL)
			continue;
		obj_hurtbox = static_cast<Hurtbox*>(curr_obj->HasComponent("HURTBOX"));
		if (obj_hurtbox == NULL)
			continue;
		if (Collision::AABB(hitbox, obj_hurtbox->GetPosition())) {
			p_event_manager->QueueTimedEvent(
				new HitEvent(punch_damage, obj_hurtbox->GetOwner()->index)
			);
		}
	}

	//Skip of self game object so do in two loops, one before self and one after self

	for (unsigned int i = GetOwner()->index + 1; i < pGameObjectManager->max_objects; i++) {
		curr_obj = pGameObjectManager->game_object_list[i];
		if (curr_obj == NULL)
			continue;
		obj_hurtbox = static_cast<Hurtbox*>(curr_obj->HasComponent("HURTBOX"));
		if (obj_hurtbox == NULL)
			continue;
		if (Collision::AABB(hitbox, obj_hurtbox->GetPosition())) {
			p_event_manager->QueueTimedEvent(
				new HitEvent(punch_damage, obj_hurtbox->GetOwner()->index)
			);
		}		
	}
}