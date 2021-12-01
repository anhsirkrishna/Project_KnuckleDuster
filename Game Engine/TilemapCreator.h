#pragma once

#include "Tilemap.h"
#include "ComponentCreator.h"

class TilemapCreator : public ComponentCreator {
	virtual Component* Create(json json_object);
};
