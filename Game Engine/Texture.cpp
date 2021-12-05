#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <SDL_image.h>

#include "Texture.h"

Texture::Texture(const std::string& filename) {
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());

	width = loadedSurface->w;
	height = loadedSurface->h;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	int Mode = GL_RGB;

	if (loadedSurface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, loadedSurface->w, loadedSurface->h, 0, Mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GL_REPEAT);
	//float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(loadedSurface);
}

// Makes a texture accessible to a program specified by the program_id
// The name of the sampler2D variable in the shader program is specified by name
void Texture::Bind(const int unit, const int program_id, const std::string & name)
{
	glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int loc = glGetUniformLocation(program_id, name.c_str());
	glUniform1i(loc, unit);
}

//Unbind a texture after use
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}