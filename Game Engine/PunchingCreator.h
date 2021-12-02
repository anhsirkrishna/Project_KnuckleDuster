#pragma once

#include "ComponentCreator.h"

class PunchingCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};