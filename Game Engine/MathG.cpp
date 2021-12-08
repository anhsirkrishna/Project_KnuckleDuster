#include "MathG.h"

float DistanceSquared(SDL_Rect pos_a, SDL_Rect pos_b) {
	return (((pos_b.y - pos_a.y) * (pos_b.y - pos_a.y)) + ((pos_b.x - pos_a.x) * (pos_b.x - pos_a.x)));
}

float Distance(SDL_Rect pos_a, SDL_Rect pos_b) {
	return sqrt(DistanceSquared(pos_a, pos_b));
}