#pragma once

#include "Component.h"
class Hurtbox;
class Animation;

class Jumpkick : public Component {
private:
	int damage;
	int move_distance;
	SDL_Rect hitbox;
	Hurtbox* p_owner_hurtbox;
	Animation* p_owner_animation;
	int hitbox_x_offset;
	int hitbox_y_offset;
	int hit_distance;
	std::string prev_state;
public:
	Jumpkick();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	void Kick();
	void CheckCollision();
	void UpdateHitbox();
};