#include "HurtboxCreator.h"
#include "Hurtbox.h"

Component* HurtboxCreator::Create(json json_object) {
	Hurtbox* new_hurtbox = new Hurtbox;
	new_hurtbox->Serialize(json_object);
	return static_cast<Component*>(new_hurtbox);
}