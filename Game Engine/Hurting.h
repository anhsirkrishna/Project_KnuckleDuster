#pragma once
#include "Component.h"

class Hurtbox;
class GLSprite;
class Animation;
class TimedEvent;

class Hurting : public Component {
public:
	Hurting();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	virtual void HandleEvent(TimedEvent* p_event);
	void Hurt(int damage, int direction, int hit_distance);
private:
	Hurtbox* p_owner_hurtbox;
	GLSprite* p_owner_glsprite;
	Animation* p_owner_animation;
	float move_distance;
};