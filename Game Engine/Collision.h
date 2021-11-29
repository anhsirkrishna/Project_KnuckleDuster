#pragma once

#include "SDL.h"
#include "Events.h"

class Collision {
public:
	static bool AABB(SDL_Rect rect1, SDL_Rect rect2);
};
