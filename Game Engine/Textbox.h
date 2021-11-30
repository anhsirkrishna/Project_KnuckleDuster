#pragma once

#include "Component.h"
#include "Matrix3D.h"
#include "GL/glew.h"

class Texture;

class Textbox : public Component {
public:
	Textbox(float x, float y, float width, float height, std::string _text, GLfloat _font_size=1);
	void Draw(ShaderProgram* p_program);
	void Drawcharacter(ShaderProgram* p_program, int grid_w, int grid_h, int char_index);
	void CalculateTexOffset(int char_index, GLfloat& tex_offset_x, GLfloat& tex_offset_y);
	void Toggle();
private:
	SDL_Rect dimensions;
	std::string text;
	GLfloat font_size;
	unsigned int grid_width;
	unsigned int grid_height;
	bool visible;
	GLfloat color_coords[16];
	GLfloat tex_coords[8];
	GLfloat tex_coords_offset[18];
	Texture* p_texture;
	Matrix3D translate_matrix;
	GLuint vao_id;
	const float quad_width = 8.07692307;
	const int quad_height = 16;
};