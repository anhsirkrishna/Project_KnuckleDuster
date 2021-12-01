#include "TilemapCreator.h"

Component* TilemapCreator::Create(json json_object) {
	Tilemap* new_tilemap = new Tilemap;
	new_tilemap->Serialize(json_object);
	return static_cast<Component*>(new_tilemap);
}