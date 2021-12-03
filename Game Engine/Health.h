#pragma once

#include "Component.h"

class Health : public Component {
private:
	unsigned int max_health;
	unsigned int current_health;
public:
	Health();
	void Serialize(json json_object);
	virtual void HandleEvent(TimedEvent* p_event);
	void TakeDamage(unsigned int damage);
	void Heal(unsigned int heal);
};