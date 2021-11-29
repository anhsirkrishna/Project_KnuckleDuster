#pragma once

#include "GLSprite.h"
#include "ComponentCreator.h"

class GLSpriteCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};