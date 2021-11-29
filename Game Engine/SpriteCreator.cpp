#include "SpriteCreator.h"

Component* SpriteCreator::Create(json json_object) {
	Sprite* new_sprite = new Sprite;
	new_sprite->Serialize(json_object);
	return static_cast<Component*>(new_sprite);
}