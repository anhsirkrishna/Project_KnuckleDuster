#pragma once

#include "Component.h"

class GLSprite;
class Transform;

class Tilemap : public Component {
private:
	GLSprite* p_owner_sprite;
	Transform* p_owner_transform;
	int grid_width;
	int grid_height;
	int tile_width;
	int tile_height;
	SDL_Rect dimensions;

	std::vector<std::vector<std::vector<int>>> tile_index_map;
public:
	Tilemap();
	void Serialize(json json_object);
	virtual void Link();
	virtual void Draw(ShaderProgram* p_program);
};