#pragma once

#include "Transform.h"
#include "ComponentCreator.h"

class TransformCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};