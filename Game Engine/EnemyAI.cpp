#include "EnemyAI.h"
#include "Walking.h"
#include "Hurtbox.h"
#include "Punching.h"
#include "GameObjectManager.h"


EnemyAI::EnemyAI() : Component("EnemyAI"), p_owner_hurtbox(NULL), p_owner_punching(NULL), p_owner_walking(NULL), p_hostage(NULL), p_player(NULL), player_agro_range(0) {}

void EnemyAI::Serialize(json json_object) {
	player_agro_range = json_object["player_agro_range"].get<int>();
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
			continue;
		}

		if (pGameObjectManager->game_object_list[i]->HasComponent("CONTROLLER")) {
			p_player = pGameObjectManager->game_object_list[i];
			continue;
		}

		if (p_hostage == NULL || p_player == NULL)
			break;
	}
}

void EnemyAI::Update() {
	if (CheckPlayerInRange())
}

bool CheckPlayerInRange() {
	if 
}