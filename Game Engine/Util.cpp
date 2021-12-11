#include "Util.h"
#include "GameManager.h"

#include "SDL.h"
//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(GLfloat* tex_coords, float tex_width, float tex_height) {
	unsigned int size = 8;
	for (unsigned int i = 0; i < size; i += 2) {
		tex_coords[i] = tex_coords[i] / tex_width;
		tex_coords[i + 1] = tex_coords[i + 1] / tex_height;
	}
}

void DEBUG_LOG(std::string const log_text) {
	if (p_game_manager->Debug() == 0) {
		SDL_Log(log_text.c_str());
	}
}