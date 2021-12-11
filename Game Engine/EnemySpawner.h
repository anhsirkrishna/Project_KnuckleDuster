#pragma once
#include "Component.h"

class EnemySpawner : public Component {
private:
	int total_enemy_count;
	int enemies_spawned;
	int current_enemy_count;
	int max_enemy_count;
	std::string enemy_obj_def;
	int spawn_index;
	std::vector<std::vector<int>> spawn_positions;
	int spawn_timer, spawn_time;
	std::vector<int> enemy_obj_index;
	std::vector<std::string> enemy_types;
	std::vector<int> enemy_spawn_chance;
public:
	EnemySpawner();
	void Serialize(json json_object);
	virtual void Link();
	virtual void Update();
	virtual void HandleEvent(TimedEvent* p_event);
	void SpawnNewEnemy();
	bool Finished();
};