#include "ComponentCreator.h"
#include "Health.h"
#include "Knockdown.h"
#include "Playerdazed.h"
#include "Jumpkick.h"
#include "Hostage.h"
#include "EnemyAI.h"
#include "EnemySpawner.h"

Component* ComponentCreator::Create(json json_object) {
	return NULL;
}

Component* HealthCreator::Create(json json_object) {
	Health* new_health = new Health;
	new_health->Serialize(json_object);
	return static_cast<Component*>(new_health);
}

Component* KnockdownCreator::Create(json json_object) {
	Knockdown* new_kd = new Knockdown;
	new_kd->Serialize(json_object);
	return static_cast<Component*>(new_kd);
}

Component* PlayerdazedCreator::Create(json json_object) {
	Playerdazed* new_pd = new Playerdazed;
	new_pd->Serialize(json_object);
	return static_cast<Component*>(new_pd);
}

Component* JumpkickCreator::Create(json json_object) {
	Jumpkick* new_jk = new Jumpkick;
	new_jk->Serialize(json_object);
	return static_cast<Component*>(new_jk);
}

Component* HostageCreator::Create(json json_object) {
	Hostage* new_hostage = new Hostage;
	new_hostage->Serialize(json_object);
	return static_cast<Component*>(new_hostage);
}

Component* EnemyAICreator::Create(json json_object) {
	EnemyAI* new_ai = new EnemyAI;
	new_ai->Serialize(json_object);
	return static_cast<Component*>(new_ai);
}

Component* EnemySpawnerCreator::Create(json json_object) {
	EnemySpawner* new_es = new EnemySpawner;
	new_es->Serialize(json_object);
	return static_cast<Component*>(new_es);
}