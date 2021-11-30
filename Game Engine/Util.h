#pragma once

#include "GL\glew.h"

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(GLfloat* tex_coords, float tex_width, float tex_height);