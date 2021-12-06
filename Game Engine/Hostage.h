#pragma once
#include "Component.h"

class GLSprite;

class Hostage : public Component {
private:
	GLSprite* p_owner_glsprite;
public:
	Hostage();
	void Serialize(json json_object);
	virtual void HandleEvent(TimedEvent* p_event);
	virtual void Link();
};