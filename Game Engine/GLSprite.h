#pragma once
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <vector>
#include "Component.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Texture.h"
#include "GameObject.h"

class GLSprite : public Component {
public:
	GLSprite();
	Texture* GetTexture();
	void SetTexture(Texture* _p_texture);
	void Serialize(json json_object);
	void Draw(ShaderProgram* program);
	void SetTexOffset(GLfloat tex_offset_x, GLfloat tex_offset_y);
	virtual void Link();
private:
	Texture* p_texture;
	GLuint vao_id = 0;
	GLuint vertex_count;
	Transform* p_owner_transform;
	GLfloat tex_offset[2];
};