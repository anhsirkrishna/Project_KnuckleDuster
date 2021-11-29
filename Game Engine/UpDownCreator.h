#pragma once

#include "UpDown.h"
#include "ComponentCreator.h"

class UpDownCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};