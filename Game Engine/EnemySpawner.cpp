#include "EnemySpawner.h"
#include "GameObjectFactory.h"
#include "Hurtbox.h"
#include "Game.h"
#include "EventManager.h"
#include "FrameRateController.h"

EnemySpawner::EnemySpawner() : Component("ENEMYSPAWNER"), total_enemy_count(0), current_enemy_count(0), enemies_spawned(0),
							   max_enemy_count(0), spawn_index(0), spawn_timer(0), spawn_time(0) {}

void EnemySpawner::Serialize(json json_object) {
	total_enemy_count = json_object["total_enemy_count"].get<int>();
	max_enemy_count = json_object["max_enemy_count"].get<int>();
	enemy_obj_def = json_object["enemy_obj_def"].get<std::string>();
	spawn_positions = json_object["spawn_positions"].get<std::vector<std::vector<int>>>();
	spawn_time = json_object["spawn_time"].get<int>();
}

void EnemySpawner::Link() {
	p_event_manager->SubscribeToEvent(EventID::downed, GetOwner());
}

void EnemySpawner::Update() {
	if (enemies_spawned == total_enemy_count)
		return;

	if (spawn_timer > 0) {
		spawn_timer -= pFrameRateController->GetPrevLoopDeltaTime();
		return;
	}

	if (current_enemy_count < max_enemy_count) {
		SpawnNewEnemy();
		spawn_timer = spawn_time;
	}	
}

void EnemySpawner::SpawnNewEnemy() {
	std::string enemy_name = "Enemy_" + std::to_string(enemies_spawned);
	GameObject* new_enemy_obj = GameObjectFactory().CreateGameObject(enemy_name, enemy_obj_def);
	new_enemy_obj->ChangeState("IDLE");
	new_enemy_obj->LinkComponents();
	pGameObjectManager->AddGameObject(new_enemy_obj);

	Hurtbox* p_enemy_hurtbox = static_cast<Hurtbox*>(new_enemy_obj->HasComponent("HURTBOX"));
	SDL_Rect curr_position = p_enemy_hurtbox->GetPosition();
	curr_position.x = spawn_positions[spawn_index][0];
	curr_position.y = spawn_positions[spawn_index][1];
	p_enemy_hurtbox->SetPosition(curr_position);
	spawn_index = (spawn_index + 1) % spawn_positions.size();

	current_enemy_count++;
	enemies_spawned++;
}

void EnemySpawner::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::downed:
		current_enemy_count--;
		break;
	}
}

bool EnemySpawner::Finished() {
	return ((enemies_spawned == total_enemy_count) && (current_enemy_count == 0));
}