#include "GLSpriteCreator.h"

Component* GLSpriteCreator::Create(json json_object) {
	GLSprite* new_sprite = new GLSprite;
	new_sprite->Serialize(json_object);
	return static_cast<Component*>(new_sprite);
}