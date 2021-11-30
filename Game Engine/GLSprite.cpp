#include "GLSprite.h"

/*Helper function to convert colors from 0-255 range to 0-1 range*/
void ConvertColor(std::vector<float>& colors) {
	unsigned int size = colors.size();
	for (unsigned int i = 0; i < size; i++) {
		colors[i] = colors[i] / 255.0;
	}
}

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(std::vector<float>& tex_coords, float tex_width, float tex_height) {
	unsigned int size = tex_coords.size();
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

GLSprite::GLSprite() : Component("GLSPRITE"), p_texture(NULL), vao_id(0), vertex_count(0), p_owner_transform(NULL) { }

Texture* GLSprite::GetTexture() {
	return p_texture;
}

void GLSprite::SetTexture(Texture* _p_texture) {
	p_texture = _p_texture;
}

void GLSprite::Draw(ShaderProgram* program) {
	GLuint loc = glGetUniformLocation(program->program_id, "translateMatrix");
	Matrix3D translate_matrix = p_owner_transform->GetTranslateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, translate_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "rotateMatrix");
	Matrix3D rotate_matrix = p_owner_transform->GetRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, rotate_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "preRotateMatrix");
	Matrix3D pre_rotate_matrix = p_owner_transform->GetPreRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, pre_rotate_matrix.GetMatrixP());
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "postRotateMatrix");
	Matrix3D post_rotate_matrix = p_owner_transform->GetPostRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, post_rotate_matrix.GetMatrixP());
	CHECKERROR;

	glActiveTexture(GL_TEXTURE2); // Activate texture unit 2
	glBindTexture(GL_TEXTURE_2D, p_texture->texture_id); // Load texture into it
	loc = glGetUniformLocation(program->program_id, "texture_map");
	glUniform1i(loc, 2); // Tell shader texture is in unit 2
	CHECKERROR;

	glBindVertexArray(vao_id);
	CHECKERROR;
	glDrawElements(GL_TRIANGLE_FAN, vertex_count, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

void GLSprite::Serialize(json json_object) {
	auto texture_name = json_object["texture_name"].get<std::string>();
	pResourceManager->add_texture(texture_name);
	p_texture = pResourceManager->get_texture(texture_name);

	//Create a VAO and put the ID in vao_id
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Put a vertex consisting of 3 float coordinates x,y,z into the list of all vertices
	auto vertices = json_object["vertex_list"].get<std::vector<float>>();
	vertex_count = vertices.size()/3;

	//Create a continguous buffer for all the vertices/points
	GLuint point_buffer;
	glGenBuffers(1, &point_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//Put a color consisting of 4 float values rgba into the list of all colors 
	auto colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(colors);
	//Create another continuguous buffer for all the colors for each vertex
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//Put a texture coordinate cosisting of 2 uv float values 
	auto coord = json_object["tex_coord_list"].get<std::vector<float>>();
	//Convert coords from image space to 0..1
	ConvertTextureCoords(coord, p_texture->width, p_texture->height);
	//Create another continguous buffer for all the textures for each vertex
	GLuint tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coord.size(), &coord[0], GL_STATIC_DRAW);
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

void GLSprite::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}