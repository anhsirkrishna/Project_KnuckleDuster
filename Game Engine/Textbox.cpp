
#include "Textbox.h"

#include "Texture.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "Util.h"

#include <cctype>

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line GLSprite.cpp:%d): %s\n", __LINE__, glewGetErrorString(err)); exit(-1);} }

Textbox::Textbox(float x, float y, float width, float height, 
				std::string _text, GLfloat _font_size) : Component("PANEL"), text(_text), grid_width(width), grid_height(height),
														 visible(true), vao_id(0), font_size(_font_size) {
	dimensions.x = x;
	dimensions.y = y;
	dimensions.w = width * quad_width;
	dimensions.h = height * quad_height;

	for (unsigned int i = 0; i < 16; ++i)
		color_coords[i] = 1.0;

	pResourceManager->add_texture("font");
	p_texture = pResourceManager->get_texture("font");

	//Texture coordiantes taken according to font_numbered_16.png (check resources for reference)
	//Tex coord upper left
	tex_coords[0] = 0;
	tex_coords[1] = 0;

	//Tex coord lower left
	tex_coords[2] = 0;
	tex_coords[3] = quad_height;

	//Tex coord lower right
	tex_coords[4] = quad_width;
	tex_coords[5] = quad_height;

	//Tex coord upper left
	tex_coords[6] = quad_width;
	tex_coords[7] = 0;

	tex_coords_offset[0] = tex_coords_offset[1] = 0.0;

	//Create a VAO and put the ID in vao_id
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Put a vertex consisting of 3 float coordinates x,y,z into the list of all vertices
	//Specifying the quad that represents one section this panel in terms of 4 vertices
	GLfloat vertices[12] = { 0 };
	vertices[6] = vertices[9] = quad_width*font_size;
	vertices[4] = vertices[7] = quad_height*font_size;

	//Create a continguous buffer for all the vertices/points
	GLuint point_buffer;
	glGenBuffers(1, &point_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Create another continuguous buffer for all the colors for each vertex
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, &color_coords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//Put a texture coordinate cosisting of 2 uv float values 
	//Convert coords from image space to 0..1
	ConvertTextureCoords(tex_coords, p_texture->width, p_texture->height);
	//Create another continguous buffer for all the textures for each vertex
	GLuint tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, &tex_coords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };
	//Create IBO
	GLuint indeces_buffer;
	glGenBuffers(1, &indeces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	CHECKERROR;
	glBindVertexArray(0);
}

void Textbox::Toggle() {
	visible = ~visible;
}

void Textbox::Draw(ShaderProgram* p_program) {
	if (!visible)
		return;

	glBindVertexArray(vao_id);
	CHECKERROR;
	Matrix3D id_matrix;

	glActiveTexture(GL_TEXTURE2); // Activate texture unit 2
	glBindTexture(GL_TEXTURE_2D, p_texture->texture_id); // Load texture into it
	GLuint loc = glGetUniformLocation(p_program->program_id, "texture_map");
	glUniform1i(loc, 2); // Tell shader texture is in unit 2
	CHECKERROR;

	loc = glGetUniformLocation(p_program->program_id, "rotateMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, id_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(p_program->program_id, "scaleMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, id_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(p_program->program_id, "preRotateMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, id_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(p_program->program_id, "postRotateMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, id_matrix.GetMatrixP());
	CHECKERROR;

	//Draw each section of the panel individually
	int char_index = 0;
	for (int i = 0; i < grid_height; i++) {
		for (int j = 0; j < grid_width; j++) {
			Drawcharacter(p_program, j, i, char_index);
			char_index++;
			if (char_index == text.size()) {
				glBindVertexArray(0);
				return;
			}
		}
	}

	glBindVertexArray(0);
}

//Draws a section of the panel at the grid position grid_w, grid_h
void Textbox::Drawcharacter(ShaderProgram* p_program, int grid_w, int grid_h, int char_index) {
	GLuint loc;

	if (isspace(text[char_index]))
		return;
	//dimensions.xy is the top left corner of the panel
	//each section is an offset of 16 pixels
	translate_matrix.SetVal(0, 3, dimensions.x + (grid_w * ((quad_width*font_size)+2)));
	translate_matrix.SetVal(1, 3, dimensions.y + (grid_h * quad_height*font_size));

	GLfloat tex_offset[2];
	GLfloat tex_offset_x, tex_offset_y;
	CalculateTexOffset(char_index, tex_offset_x, tex_offset_y);
	tex_offset[0] = tex_offset_x / p_texture->width;
	tex_offset[1] = tex_offset_y / p_texture->height;

	loc = glGetUniformLocation(p_program->program_id, "translateMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, translate_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(p_program->program_id, "tex_offset");
	glUniform2fv(loc, 1, &(tex_offset[0]));
	CHECKERROR;

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

}

void Textbox::CalculateTexOffset(int char_index, GLfloat& tex_offset_x, GLfloat& tex_offset_y) {
	tex_offset_y = 0.0;
	char c = text[char_index];
	//Characters in the texture are arranged alphabetically so A would be in the 0th position in the texture
	//Ascii value of A is 65. 65-65 = 0 hence it will be the 0th position
	int texture_index = int(c) - 65;
	tex_offset_x = texture_index * quad_width;
}