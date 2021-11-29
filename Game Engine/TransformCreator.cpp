#include "TransformCreator.h"

Component* TransformCreator::Create(json json_object) {
	Transform* new_transform = new Transform;
	new_transform->Serialize(json_object);
	return static_cast<Component*>(new_transform);
}