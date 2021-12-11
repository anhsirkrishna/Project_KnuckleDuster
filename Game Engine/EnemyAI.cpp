#include "EnemyAI.h"
#include "Walking.h"
#include "Hurtbox.h"
#include "Punching.h"
#include "GameObjectManager.h"
#include "MathG.h"
#include "FrameRateController.h"
#include "Events.h"

EnemyAI::EnemyAI() : Component("ENEMYAI"), p_owner_hurtbox(NULL), p_owner_punching(NULL), p_owner_walking(NULL), p_hostage(NULL), 
					p_player(NULL), p_target(NULL), player_agro_range(0), attack_range(0), action_cooldown(0), action_timer(0),
					is_downed(false) {}

void EnemyAI::Serialize(json json_object) {
	player_agro_range = json_object["player_agro_range"].get<int>();
	attack_range = json_object["attack_range"].get<int>();
	action_cooldown = json_object["action_cooldown"].get<int>();
}

void EnemyAI::Link() {
	p_owner_punching = static_cast<Punching*>(GetOwner()->HasComponent("PUNCHING"));
	p_owner_walking = static_cast<Walking*>(GetOwner()->HasComponent("WALKING"));
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
	for (unsigned int i = 0; i < pGameObjectManager->max_objects; i++) {
		if (pGameObjectManager->game_object_list[i] == NULL)
			continue;

		if (pGameObjectManager->game_object_list[i]->HasComponent("HOSTAGE")) {
			p_hostage = pGameObjectManager->game_object_list[i];
			p_target = p_hostage;
			continue;
		}

		if (pGameObjectManager->game_object_list[i]->HasComponent("CONTROLLER")) {
			p_player = pGameObjectManager->game_object_list[i];
			continue;
		}

		if (p_hostage != NULL && p_player != NULL)
			break;
	}
}

void EnemyAI::Update() {
	if (is_downed)
		return;

	if (GetOwner()->CurrentState() == "JUMPKICK" ||
		GetOwner()->CurrentState() == "PUNCH" ||
		GetOwner()->CurrentState() == "HURT")
		return;
	if (action_timer > 0) {
		action_timer -= pFrameRateController->GetPrevLoopDeltaTime();
		return;
	}
		
	if (CheckTargetInRange(p_player, player_agro_range))
		p_target = p_player;
	else
		p_target = p_hostage;

	if (CheckTargetInAttackRange(p_target)) {
		p_owner_punching->Punch();
		action_timer = action_cooldown;
	}
	else
		WalkToTarget();
}

bool EnemyAI::CheckTargetInRange(GameObject* _target, int _range) {
	Hurtbox* target_hurtbox = static_cast<Hurtbox*>(_target->HasComponent("HURTBOX"));
	if (Distance(p_owner_hurtbox->GetPosition(), target_hurtbox->GetPosition()) < _range)
		return true;

	return false;
}

bool EnemyAI::CheckTargetInAttackRange(GameObject* _target) {
	Hurtbox* target_hurtbox = static_cast<Hurtbox*>(_target->HasComponent("HURTBOX"));
	SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
	SDL_Rect target_curr_position = target_hurtbox->GetPosition();
	if ((target_curr_position.x + target_curr_position.w + (attack_range / 3)) < curr_position.x)
		return false;
	else if (target_curr_position.x > (curr_position.x + curr_position.w + (attack_range / 3)))
		return false;

	if ((target_curr_position.y + (target_curr_position.h / 4)) < curr_position.y)
		return false;
	else if (target_curr_position.y > (curr_position.y + (curr_position.h / 4)))
		return false;
	
	return true;
}

void EnemyAI::WalkToTarget() {
	Hurtbox* target_hurtbox = static_cast<Hurtbox*>(p_target->HasComponent("HURTBOX"));
	SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
	SDL_Rect target_curr_position = target_hurtbox->GetPosition();
	if ((target_curr_position.x + target_curr_position.w + (attack_range/3)) < curr_position.x)
		p_owner_walking->Walk(WalkDirection::Left);
	else if (target_curr_position.x > (curr_position.x + curr_position.w + (attack_range/3)))
		p_owner_walking->Walk(WalkDirection::Right);
	else
		p_owner_walking->SetDirXZero();

	if ((target_curr_position.y + (target_curr_position.h / 4)) < curr_position.y)
		p_owner_walking->Walk(WalkDirection::Up);
	else if (target_curr_position.y > (curr_position.y + (curr_position.h / 4)))
		p_owner_walking->Walk(WalkDirection::Down);
	else
		p_owner_walking->SetDirYZero();
}

void EnemyAI::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::health_zero:
		is_downed = true;
		break;
	}
}