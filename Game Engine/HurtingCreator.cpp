#include "HurtingCreator.h"
#include "Hurting.h"

Component* HurtingCreator::Create(json json_object) {
	Hurting* new_hurting = new Hurting;
	new_hurting->Serialize(json_object);
	return static_cast<Component*>(new_hurting);
}