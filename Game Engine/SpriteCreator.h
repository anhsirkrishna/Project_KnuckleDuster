#pragma once

#include "Sprite.h"
#include "ComponentCreator.h"

class SpriteCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};