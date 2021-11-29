#pragma once

#include "Component.h"

class Hurtbox;

class UpDown : public Component {
public:
	virtual void Update();
	virtual void Link();
	void Serialize(json json_object);
	virtual void HandleEvent(TimedEvent* p_event);
private:
	unsigned int move_rate;
	int range;
	int direction = 1;
	bool subscribed;
	bool move;
	Hurtbox* p_owner_hurtbox;
};