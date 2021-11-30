#include "UpDown.h"
#include "GameObject.h"
#include "Hurtbox.h"
#include "EventManager.h"

UpDown::UpDown() : Component("UPDOWN"), move_rate(0), range(0), direction(1), 
					subscribed(false),  move(true), p_owner_hurtbox(NULL) {}

void UpDown::Update() {
	if (move) {
		SDL_Rect currPosition = p_owner_hurtbox->GetPosition();
		currPosition.y += direction * move_rate;

		if (std::abs(currPosition.y) >= range || std::abs(currPosition.y) <= 0)
			direction *= -1; //Start moving in the other direction
		p_owner_hurtbox->SetPosition(currPosition);
	}
}

void UpDown::Link() {
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
	//Check if this object needs to subscribe to collision events 
	if (subscribed) {
		p_event_manager->SubscribeToEvent(EventID::collision, GetOwner());
	}
}

void UpDown::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::collision:
		//Toggle movement in case of collision
		SDL_Rect currPosition = p_owner_hurtbox->GetPosition();
		currPosition.x += 5;
		p_owner_hurtbox->SetPosition(currPosition);
		break;
	}
}

void UpDown::Serialize(json json_object) {
	move_rate = json_object["move_rate"].get<int>();
	range = json_object["range"].get<int>();
	subscribed = json_object["subscribed"].get<bool>();
	move = true;
}