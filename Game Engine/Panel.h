#pragma once
#include "Component.h"
#include "Matrix3D.h"
#include "GL/glew.h"

class Texture;

class Panel : public Component {
	Panel(float x, float y, float width, float height);
	void Draw(ShaderProgram* p_program);
	void DrawSection(ShaderProgram* p_program, int grid_w, int grid_h);
	void CalculateTexOffset(int grid_w, int grid_h, GLfloat& tex_offset_x, GLfloat& tex_offset_y);
	void Toggle();
private:
	SDL_Rect dimensions;
	unsigned int grid_width;
	unsigned int grid_height;
	bool visible;
	GLfloat color_coords[16];
	GLfloat tex_coords[8];
	GLfloat tex_coords_offset[18];
	Texture* p_texture;
	Matrix3D translate_matrix;
	GLuint vao_id;
	
};