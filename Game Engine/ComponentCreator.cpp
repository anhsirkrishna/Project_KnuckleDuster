#include "ComponentCreator.h"
#include "Health.h"

Component* ComponentCreator::Create(json json_object) {
	return NULL;
}

Component* HealthCreator::Create(json json_object) {
	Health* new_health = new Health;
	new_health->Serialize(json_object);
	return static_cast<Component*>(new_health);
}