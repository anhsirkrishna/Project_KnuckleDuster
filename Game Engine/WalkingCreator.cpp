#include "WalkingCreator.h"
#include "Walking.h"

Component* WalkingCreator::Create(json json_object) {
	Walking* new_walking = new Walking;
	new_walking->Serialize(json_object);
	return static_cast<Component*>(new_walking);
}