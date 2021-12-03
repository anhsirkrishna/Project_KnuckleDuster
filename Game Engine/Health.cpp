#include "Health.h"
#include "EventManager.h"
#include "GameObject.h"

Health::Health() : Component("HEALTH"), max_health(0), current_health(0) {}

void Health::Serialize(json json_object) {
	max_health = json_object["max_health"].get<int>();
	current_health = max_health;
}

void Health::TakeDamage(unsigned int damage) {
	current_health = (current_health - damage) > 0 ? (current_health - damage) : 0;
	if (current_health == 0) {
		p_event_manager->QueueTimedEvent(
			new TimedEvent(EventID::health_zero, false, GetOwner()->index)
		);
	}
}

void Health::Heal(unsigned int heal) {
	current_health = (current_health + heal) <= max_health ? (current_health + heal) : max_health;
}

void Health::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::take_damage:
		TakeDamageEvent* p_take_damage_event = static_cast<TakeDamageEvent*>(p_event);
		TakeDamage(p_take_damage_event->hit_damage);
		break;
	}
}