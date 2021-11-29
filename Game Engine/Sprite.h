#pragma once

#include "Component.h"
#include "SDL_surface.h"
#include "ResourceManager.h"
#include "Transform.h"

class Sprite : public Component {
public:
	SDL_Surface* GetImage();
	void SetImage(SDL_Surface* p_sprite);
	void Serialize(json json_object);
	void Draw(SDL_Surface* surface);
private:
	SDL_Surface* p_sprite_image;
};