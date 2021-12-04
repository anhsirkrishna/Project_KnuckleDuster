#pragma once

#include "Component.h"

class Textbox;
class Texture;

class Button : public Component {
private:
	SDL_Rect dimensions;
	Textbox* p_textbox;
	Texture* p_texture;
	GLfloat color_coords[16];
	GLfloat tex_coords[8];
	GLfloat tex_coords_offset[18];
	void (*button_action)();
	GLuint vao_id;
	bool visible;
	bool button_down;
	int grid_width;
	const int quad_width = 16;
	const int quad_height = 16;
public:
	Button(int x, int y, int w, std::string text, void (*_button_action)());
	//virtual void Update();
	virtual void Draw(ShaderProgram* p_program);
	virtual void Update();
	void Click();
};