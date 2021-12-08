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
	int player_agro_range;
public:
	EnemyAI();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	bool CheckPlayerInRange();
};