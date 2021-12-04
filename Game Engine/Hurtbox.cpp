#include "Game.h"
#include "Hurtbox.h"
#include "GameObject.h"
#include "Collision.h"
#include "EventManager.h"
#include "GameObjectManager.h"

Hurtbox::Hurtbox() : Component("HURTBOX"), box(),  p_owner_transform(NULL), scale_x(1), scale_y(1), solid(true) {}

SDL_Rect Hurtbox::GetPosition() {
	return box;
}

float Hurtbox::GetScaleX() {
	return scale_x;
}

float Hurtbox::GetScaleY() {
	return scale_y;
}

void Hurtbox::SetScale(float _scale_x, float _scale_y) {
	scale_x = _scale_x;
	scale_y = _scale_y;
}


void Hurtbox::SetPosition(SDL_Rect new_position) {
	box = new_position;
}

void Hurtbox::SetSolid(bool _solid) {
	solid = _solid;
}

bool Hurtbox::IsSolid() {
	return solid;
}

void Hurtbox::Update() {
	if (CheckOutOfBounds()) {
		//Reset the hurtbox back to it's previous position since it's gone out of bounds
		box = p_owner_transform->GetPosition();
	}

	if (CheckCollision()) {
		//Reset the hurtbox back to it's previous position since it's collided with another hurtbox
		box = p_owner_transform->GetPosition();
		/*
		p_event_manager->QueueTimedEvent(new TimedEvent(EventID::collision));
		//Set the broadcast event to trigger after 2 seconds 
		TimedEvent* delayed_broadcast_event = new TimedEvent(EventID::collision, true);
		delayed_broadcast_event->SetTime(2000);
		p_event_manager->QueueTimedEvent(delayed_broadcast_event);
		*/
	}
	
	p_owner_transform->SetPosition(box);
	p_owner_transform->SetScale(scale_x, scale_y);
}

//Function to check if the Hurtbox has left the world bounds
bool Hurtbox::CheckOutOfBounds() {
	//Check if we've gone too far left or right or up or down 
	if (box.x < 0 || 
		(box.x + box.w) >= WORLD_WIDTH ||
		box.y < 0 || 
		box.y + box.h >= WORLD_HEIGHT) 
		return true;
	
	return false;
}

//Function to check if a collision has occured with another hurtbox 
bool Hurtbox::CheckCollision() {
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
		if (Collision::AABB(box, obj_hurtbox->box))
			return true;
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
		if (Collision::AABB(box, obj_hurtbox->box))
			return true;
	}

	return false;
}

void Hurtbox::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_transform->SetPosition(box);
	p_owner_transform->SetRotMatrices();

	if (GetOwner()->HasComponent("CONTROLLER")) {
		p_event_manager->SubscribeToEvent(EventID::out_of_bounds, GetOwner());
		//p_event_manager->SubscribeToEvent(EventID::collision, GetOwner());
	}
}

void Hurtbox::Serialize(json json_object) {
	auto coordinates = json_object["coordinates"].get<std::vector<int>>();
	box.x = coordinates[0];
	box.y = coordinates[1];
	box.w = coordinates[2];
	box.h = coordinates[3];
}

void Hurtbox::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::out_of_bounds:
		box.x = 200;
		box.y = 200;
		break;
	}
}