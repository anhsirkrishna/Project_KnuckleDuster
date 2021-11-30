#include "Panel.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(GLfloat* tex_coords, float tex_width, float tex_height) {
	unsigned int size = 8;
	for (unsigned int i = 0; i < size; i += 2) {
		tex_coords[i] = tex_coords[i] / tex_width;
		tex_coords[i + 1] = tex_coords[i + 1] / tex_height;
	}

	float temp;
	for (unsigned int i = 0; i < size; i += size / 2) {
		temp = tex_coords[i];
		tex_coords[i] = tex_coords[i + 2];
		tex_coords[i + 2] = temp;

		temp = tex_coords[i + 1];
		tex_coords[i + 1] = tex_coords[i + 3];
		tex_coords[i + 3] = temp;
	}
}

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line GLSprite.cpp:%d): %s\n", __LINE__, glewGetErrorString(err)); exit(-1);} }

Panel::Panel(float x, float y, float width, float height) : Component("PANEL"), grid_width(width), grid_height(height),
															visible(true), vao_id(0) {
	dimensions.x = x;
	dimensions.y = y;
	dimensions.w = width*16;
	dimensions.h = height*16;

	for (unsigned int i = 0; i < 16; ++i)
		color_coords[i] = 1.0;

	pResourceManager->add_texture("Panel");
	p_texture = pResourceManager->get_texture("Panel");

	//Texture coordiantes taken according to GUI_numbered_16.png (check resources for reference)
	//Tex coord upper left
	tex_coords[0] = 0;
	tex_coords[1] = 32;

	//Tex coord lower left
	tex_coords[2] = 0;
	tex_coords[3] = 48;

	//Tex coord lower right
	tex_coords[4] = 16;
	tex_coords[5] = 48;

	//Tex coord upper left
	tex_coords[4] = 16;
	tex_coords[5] = 32;
	
	tex_coords_offset[0] = tex_coords_offset[1] = 0.0;

	GLfloat temp_offset_x = 0, temp_offset_y = 0;
	for (unsigned int i = 0; i < 18; i+=2) {
		tex_coords_offset[i] = temp_offset_x;
		tex_coords_offset[i + 1] = temp_offset_y;
		temp_offset_x += 16;
		if ((i+2) % 3 == 0) {
			temp_offset_x -= 32;
			temp_offset_y += 16;
		}
	}

	translate_matrix.SetVal(0, 3, dimensions.x);
	translate_matrix.SetVal(1, 3, dimensions.y);

	//Create a VAO and put the ID in vao_id
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Put a vertex consisting of 3 float coordinates x,y,z into the list of all vertices
	//Specifying the quad that represents one section this panel in terms of 4 vertices
	GLfloat vertices[12] = { 0 };
	vertices[4] = vertices[7] = 16;
	vertices[6] = vertices[9] = 16;
	

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

void Panel::Toggle() {
	visible = ~visible;
}

void Panel::Draw(ShaderProgram* p_program) {
	GLuint loc;

	glActiveTexture(GL_TEXTURE2); // Activate texture unit 2
	glBindTexture(GL_TEXTURE_2D, p_texture->texture_id); // Load texture into it
	loc = glGetUniformLocation(p_program->program_id, "texture_map");
	glUniform1i(loc, 2); // Tell shader texture is in unit 2
	CHECKERROR;
	
	loc = glGetUniformLocation(p_program->program_id, "translateMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, translate_matrix.GetMatrixP());
	CHECKERROR;

	glBindVertexArray(vao_id);
	CHECKERROR;
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

//Draws a section of the panel at the grid position grid_w, grid_h
void Panel::DrawSection(ShaderProgram* p_program, int grid_w, int grid_h) {
	translate_matrix.SetVal(0, 3, dimensions.x + (grid_w * 16));
	translate_matrix.SetVal(1, 3, dimensions.y + (grid_w * 16));
	
	GLfloat tex_offset_x, tex_offset_y;
	CalculateTexOffset(grid_w, grid_h, tex_offset_x, tex_offset_y);

	GLfloat tex_offset[2];
	tex_offset[0] = tex_offset_x;
	tex_offset[1] = tex_offset_y;
	
}

void Panel::CalculateTexOffset(int grid_w, int grid_h, GLfloat& tex_offset_x, GLfloat& tex_offset_y) {
	if (grid_w == 0 && grid_h == 0) { //Upper left corner texture 
		tex_offset_x = tex_coords_offset[0];
		tex_offset_y = tex_coords_offset[1];
	}
	else if (grid_w < grid_width && grid_h == 0) { //Upper middle texture 
		tex_offset_x = tex_coords_offset[2];
		tex_offset_y = tex_coords_offset[3];
	}
	else if (grid_w < grid_width && grid_h == 0) { //Upper right corner texture 
		tex_offset_x = tex_coords_offset[4];
		tex_offset_y = tex_coords_offset[5];
	}
	else if (grid_w == 0 && grid_h < grid_height) { //Left middle texture 
		tex_offset_x = tex_coords_offset[6];
		tex_offset_y = tex_coords_offset[7];
	}
	else if (grid_w < grid_width && grid_h < grid_height) { //Middle middle texture 
		tex_offset_x = tex_coords_offset[8];
		tex_offset_y = tex_coords_offset[9];
	}
	else if (grid_w == grid_width && grid_h < grid_height) { //Right middle texture 
		tex_offset_x = tex_coords_offset[10];
		tex_offset_y = tex_coords_offset[11];
	}
	else if (grid_w == 0 && grid_h == grid_height) { //Lower left corner texture 
		tex_offset_x = tex_coords_offset[12];
		tex_offset_y = tex_coords_offset[13];
	}
	else if (grid_w < grid_width && grid_h == grid_height) { //Lower middle texture 
		tex_offset_x = tex_coords_offset[14];
		tex_offset_y = tex_coords_offset[15];
	}
	else if (grid_w == grid_width && grid_h == grid_height) { //Lower right corner texture 
		tex_offset_x = tex_coords_offset[16];
		tex_offset_y = tex_coords_offset[17];
	}
}