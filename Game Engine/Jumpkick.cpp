#include "Jumpkick.h"
#include "GameObject.h"
#include "Hurtbox.h"
#include "GLSprite.h"
#include "Animation.h"
#include "GameObjectManager.h"
#include "Collision.h"
#include "EventManager.h"


Jumpkick::Jumpkick() : Component("JUMPKICK"), p_owner_hurtbox(NULL), p_owner_animation(NULL),
					   hitbox(), damage(0), hitbox_x_offset(0), hitbox_y_offset(0), hit_distance(0), prev_state("IDLE"), move_distance(0) {}

void Jumpkick::Serialize(json json_object) {
	auto hitbox_coords = json_object["hitbox"].get<std::vector<int>>();
	hitbox_x_offset = hitbox_coords[0];
	hitbox_y_offset = hitbox_coords[1];
	hitbox.w = hitbox_coords[2];
	hitbox.h = hitbox_coords[3];

	damage = json_object["damage"].get<int>();
	move_distance = json_object["move_distance"].get<int>();
	hit_distance = json_object["hit_distance"].get<int>();
}
void Jumpkick::Link() {
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
	std::string current_state = GetOwner()->CurrentState();
	GetOwner()->ChangeState("JUMPKICK");
	p_owner_animation = static_cast<Animation*>(GetOwner()->HasComponent("ANIMATION"));
	//Get the move distance per frame
	move_distance = move_distance / p_owner_animation->Duration();
	GetOwner()->ChangeState(current_state);
}

void Jumpkick::Kick() {
	GameObject* game_obj = GetOwner();
	prev_state = game_obj->CurrentState();
	if (prev_state != "JUMPKICK") {
		GetOwner()->ChangeState("JUMPKICK");
		UpdateHitbox();
		p_owner_animation->Refresh();
	}
}

void Jumpkick::Update() {
	if (GetOwner()->CurrentState() == "JUMPKICK") {
		//Collision logic
		CheckCollision();
		SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
		curr_position.x = curr_position.x + (move_distance * p_owner_hurtbox->GetScaleX());
		p_owner_hurtbox->SetPosition(curr_position);
		UpdateHitbox();
		//Animation logic and state change
		if (p_owner_animation->Completed()) {

			GetOwner()->ChangeState("IDLE");
		}
	}
}

void Jumpkick::UpdateHitbox() {
	hitbox.x = p_owner_hurtbox->GetPosition().x + (hitbox_x_offset * p_owner_hurtbox->GetScaleX());
	hitbox.y = p_owner_hurtbox->GetPosition().y + (hitbox_y_offset * p_owner_hurtbox->GetScaleY());
}

void Jumpkick::CheckCollision() {
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
		if (!obj_hurtbox->IsSolid())
			continue;
		if (IsAlly(curr_obj))
			continue;
		if (Collision::AABB(hitbox, obj_hurtbox->GetPosition())) {
			p_event_manager->QueueTimedEvent(
				new HitEvent(damage, p_owner_hurtbox->GetScaleX(), //Hit direction is the opposite of the Jumpkick direction 
							 hit_distance, obj_hurtbox->GetOwner()->index)
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
		if (!obj_hurtbox->IsSolid())
			continue;
		if (IsAlly(curr_obj))
			continue;
		if (Collision::AABB(hitbox, obj_hurtbox->GetPosition())) {
			p_event_manager->QueueTimedEvent(
				new HitEvent(damage, p_owner_hurtbox->GetScaleX(), //Hit direction is the opposite of the Jumpkick direction 
							 hit_distance, obj_hurtbox->GetOwner()->index)
			);
		}
	}
}

bool Jumpkick::IsAlly(GameObject* p_obj) {
	if (GetOwner()->HasComponent("CONTROLLER") && p_obj->HasComponent("HOSTAGE"))
		return true;
	if (GetOwner()->HasComponent("ENEMYAI") && p_obj->HasComponent("ENEMYAI"))
		return true;
	return false;
}