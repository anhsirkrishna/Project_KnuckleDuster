#include "AnimationCreator.h"

Component* AnimationCreator::Create(json json_object) {
	Animation* new_animation = new Animation;
	new_animation->Serialize(json_object);
	return static_cast<Component*>(new_animation);
}