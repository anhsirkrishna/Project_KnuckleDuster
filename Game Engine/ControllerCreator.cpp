#include "ControllerCreator.h"

Component* ControllerCreator::Create(json json_object) {
	return static_cast<Component*>(new Controller);
}