#include "PunchingCreator.h"
#include "Punching.h"

Component* PunchingCreator::Create(json json_object) {
	Punching* new_punching = new Punching;
	new_punching->Serialize(json_object);
	return static_cast<Component*>(new_punching);
}