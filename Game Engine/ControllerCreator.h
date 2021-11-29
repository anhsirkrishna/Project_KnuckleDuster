#pragma once

#include "Controller.h"
#include "ComponentCreator.h"

class ControllerCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};
