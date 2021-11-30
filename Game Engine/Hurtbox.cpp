#include "Game.h"
#include "Hurtbox.h"
#include "GameObject.h"
#include "Collision.h"
#include "EventManager.h"


Hurtbox::Hurtbox() : Component("HURTBOX"), box(),  p_owner_transform(NULL) {}

SDL_Rect Hurtbox::GetPosition() {
	return box;
}


void Hurtbox::SetPosition(SDL_Rect new_position) {
	box = new_position;
}

//Function to get pointers to all the other hurtboxes in the game for collision detection
void Hurtbox::RegisterObjectComponents(GameObject **game_object_list, unsigned int list_size, unsigned int self_index) {
	for (unsigned int i = 0; i < self_index; i++) {
		if (game_object_list[i] == NULL)
			continue;
		Component* temp_component = game_object_list[i]->HasComponent("HURTBOX");
		if (temp_component != NULL) {
			hurtbox_list.push_back(static_cast<Hurtbox*>(temp_component));
		}
	}
	//Skip it's own hurtbox.
	for (unsigned int i = self_index+1; i < list_size; i++) {
		if (game_object_list[i] == NULL)
			continue;
		Component* temp_component = game_object_list[i]->HasComponent("HURTBOX");
		if (temp_component != NULL) {
			hurtbox_list.push_back(static_cast<Hurtbox*>(temp_component));
		}
	}
}


void Hurtbox::Update() {
	if (CheckOutOfBounds()) {
		//Reset the hurtbox back to it's previous position since it's gone out of bounds
		box = p_owner_transform->GetPosition();
		p_event_manager->QueueTimedEvent(new TimedEvent(EventID::out_of_bounds));
	}

	if (CheckCollision()) {
		//Reset the hurtbox back to it's previous position since it's collided with another hurtbox
		box = p_owner_transform->GetPosition();
		p_event_manager->QueueTimedEvent(new TimedEvent(EventID::collision));
		//Set the broadcast event to trigger after 2 seconds 
		TimedEvent* delayed_broadcast_event = new TimedEvent(EventID::collision, true);
		delayed_broadcast_event->SetTime(2000);
		p_event_manager->QueueTimedEvent(delayed_broadcast_event);
	}
	
	p_owner_transform->SetPosition(box);
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
	for (std::vector<Hurtbox*>::iterator it = hurtbox_list.begin(); it != hurtbox_list.end(); ++it) {
		if (Collision::AABB(box, (*it)->box)) {
			return true;
		}
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