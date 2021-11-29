#include "UpDownCreator.h"

Component* UpDownCreator::Create(json json_object) {
	UpDown* new_updown = new UpDown;
	new_updown->Serialize(json_object);
	return new_updown;
}