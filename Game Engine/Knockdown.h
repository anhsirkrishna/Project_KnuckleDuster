#pragma once
#include "Component.h"

class GLSprite;
class Animation;
class TimedEvent;
class Hurtbox;

class Knockdown : public Component {
private:
	Animation* p_owner_animation;
	GLSprite* p_owner_sprite;
	Hurtbox* p_owner_hurtbox;
	SDL_Rect knockdown_hurtbox;
public:
	Knockdown();
	virtual void Update();
	virtual void Link();
	virtual void HandleEvent(TimedEvent* p_event);
	void Serialize(json json_object);
	void KnockedDown();
	void GetUp();
};