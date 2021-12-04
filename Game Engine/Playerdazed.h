#pragma once
#include "Component.h"

class GLSprite;

class Playerdazed : public Component {
private:
	GLSprite* p_owner_glsprite;
	unsigned int undazed_input_count;
	unsigned int current_input_count;
	unsigned int curr_input;
	unsigned int prev_input;
public:
	Playerdazed();
	void Serialize(json json_object);
	virtual void Link();
	virtual void Update();
	virtual void HandleInput();
	virtual void HandleEvent(TimedEvent* p_event);
	void Dazed();
};