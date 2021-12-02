#pragma once

#include "ComponentCreator.h"

class HurtingCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};