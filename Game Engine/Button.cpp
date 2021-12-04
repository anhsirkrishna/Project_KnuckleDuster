#include "Textbox.h"
#include "Button.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Util.h"

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Button.cpp:%d): %s\n", __LINE__, glewGetErrorString(err)); exit(-1);} }

Button::Button(int x, int y, int w,
			   std::string text, void (*_button_action)()) : 
				Component("BUTTON"), dimensions(), button_action(_button_action), visible(true), button_down(false) {

	int button_width = w * quad_width;
	int textbox_offset_x = (button_width / 2) - ((text.size() * 15) / 2);
	p_textbox = new Textbox(x + textbox_offset_x, y + 8, text.size(), 1, text, 1.5);
	dimensions.x = x;
	dimensions.y = y;
	dimensions.w = w * quad_width;
	dimensions.h = quad_height*3;

	grid_width = w;

	for (unsigned int i = 0; i < 16; ++i)
		color_coords[i] = 1.0;

	pResourceManager->add_texture("GUI");
	p_texture = pResourceManager->get_texture("GUI");

	//Tex coord upper left
	tex_coords[0] = 0;
	tex_coords[1] = 0;

	//Tex coord lower left
	tex_coords[2] = 0;
	tex_coords[3] = quad_height;

	//Tex coord lower right
	tex_coords[4] = quad_width-1;
	tex_coords[5] = quad_height;

	//Tex coord upper left
	tex_coords[6] = quad_width-1;
	tex_coords[7] = 0;

	tex_coords_offset[0] = 9 * quad_width;
	tex_coords_offset[1] = 5 * quad_height;


	//Create a VAO and put the ID in vao_id
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Put a vertex consisting of 3 float coordinates x,y,z into the list of all vertices
	//Specifying the quad that represents one section this panel in terms of 4 vertices
	GLfloat vertices[12] = { 0 };
	vertices[6] = vertices[9] = quad_width*3;
	vertices[4] = vertices[7] = quad_height*3;

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &tex_coords[0], GL_STATIC_DRAW);
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

void Button::Click() {
	button_action();
}

void Button::Update() {
	if (pInputManager->isMouseTriggered(SDL_BUTTON_LMASK)) {
		if (pInputManager->mouse_x > dimensions.x &&
			pInputManager->mouse_x < dimensions.x + dimensions.w &&
			pInputManager->mouse_y > dimensions.y &&
			pInputManager->mouse_y < dimensions.y + dimensions.h) {
			button_down = true;
		}
	}

	if (pInputManager->isMouseReleased(SDL_BUTTON_LMASK)) {
		if (button_down == true) {
			button_down = false;
			Click();
		}			
	}

	if (button_down)
		p_textbox->SetY(dimensions.y + 12);
	else
		p_textbox->SetY(dimensions.y + 8);
}

void Button::Draw(ShaderProgram* p_program) {
	if (!visible)
		return;

	glBindVertexArray(vao_id);
	CHECKERROR;
	Matrix3D id_matrix, translate_matrix;

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

	loc = glGetUniformLocation(p_program->program_id, "translateMatrix");
	translate_matrix.SetVal(0, 3, dimensions.x);
	translate_matrix.SetVal(1, 3, dimensions.y);
	glUniformMatrix4fv(loc, 1, GL_FALSE, translate_matrix.GetMatrixP());
	CHECKERROR;

	GLfloat tex_offset[2];
	tex_offset[0] = tex_coords_offset[0] / p_texture->width;
	if (button_down) {
		tex_offset[0] = tex_offset[0];
		tex_offset[1] = (tex_coords_offset[1] + quad_height) / p_texture->height;
	}
	else
		tex_offset[1] = tex_coords_offset[1] / p_texture->height;

	loc = glGetUniformLocation(p_program->program_id, "tex_offset");
	glUniform2fv(loc, 1, &(tex_offset[0]));
	CHECKERROR;

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	unsigned int i;
	for (i = 1; i < grid_width - 1; i++) {
		translate_matrix.SetVal(0, 3, dimensions.x + (quad_width * i));
		loc = glGetUniformLocation(p_program->program_id, "translateMatrix");
		glUniformMatrix4fv(loc, 1, GL_FALSE, translate_matrix.GetMatrixP());
		CHECKERROR;

		tex_offset[0] = (tex_coords_offset[0] + quad_width) / p_texture->width;
		if (button_down) {
			tex_offset[0] = tex_offset[0];
			tex_offset[1] = (tex_coords_offset[1] + quad_height) / p_texture->height;
		}
		else
			tex_offset[1] = tex_coords_offset[1] / p_texture->height;

		loc = glGetUniformLocation(p_program->program_id, "tex_offset");
		glUniform2fv(loc, 1, &(tex_offset[0]));
		CHECKERROR;

		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	}

	translate_matrix.SetVal(0, 3, dimensions.x + (quad_width*i));
	loc = glGetUniformLocation(p_program->program_id, "translateMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, translate_matrix.GetMatrixP());
	CHECKERROR;

	tex_offset[0] = (tex_coords_offset[0] + (quad_width*2)) / p_texture->width;
	if (button_down) {
		tex_offset[0] = tex_offset[0];
		tex_offset[1] = (tex_coords_offset[1] + quad_height) / p_texture->height;
	}
	else
		tex_offset[1] = tex_coords_offset[1] / p_texture->height;

	loc = glGetUniformLocation(p_program->program_id, "tex_offset");
	glUniform2fv(loc, 1, &(tex_offset[0]));
	CHECKERROR;

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);

	p_textbox->Draw(p_program);
}