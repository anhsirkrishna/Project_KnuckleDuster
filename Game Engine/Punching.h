#pragma once
#include "Component.h"

class Hurtbox;
class GLSprite;
class Animation;

class Punching : public Component {
private:
	Hurtbox* p_owner_hurtbox;
	GLSprite* p_owner_glsprite;
	Animation* p_owner_animation;
	int sprite_index;
	SDL_Rect hitbox;
	int hitbox_x_offset;
	int hitbox_y_offset;
	int punch_damage;
	int hit_distance;
	std::string prev_state;
public:
	Punching();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	void Punch();
	void CheckCollision();
	void UpdateHitbox();
	bool IsAlly(GameObject* p_obj);
};