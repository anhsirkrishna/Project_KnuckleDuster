#pragma once

#include "ComponentCreator.h"

class WalkingCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};