#include "ComponentCreator.h"
#include "Health.h"
#include "Knockdown.h"

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