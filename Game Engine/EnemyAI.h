#pragma once
#include "Component.h"

class Walking;
class Punching;
class Hurtbox;

class EnemyAI : public Component {
private:
	Walking* p_owner_walking;
	Punching* p_owner_punching;
	Hurtbox* p_owner_hurtbox;
	GameObject* p_hostage;
	GameObject* p_player;
	GameObject* p_target;
	int player_agro_range, attack_range;
	int32_t action_cooldown, action_timer;
	bool is_downed;
public:
	EnemyAI();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	virtual void HandleEvent(TimedEvent* p_event);
	bool CheckTargetInRange(GameObject* target, int range);
	bool CheckTargetInAttackRange(GameObject* target);
	void WalkToTarget();
};