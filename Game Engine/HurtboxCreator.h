#pragma once

#include "ComponentCreator.h"

class HurtboxCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};