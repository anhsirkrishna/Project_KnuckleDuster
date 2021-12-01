#pragma once

#include "Animation.h"
#include "ComponentCreator.h"

class AnimationCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};
