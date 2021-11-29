#pragma once

#include <string>
/*
Encapsulation of an OpenGL Texture
Loads a texture from a png using SDL_Image and then stores it on the graphics card
Can bind/unbind a texture with a shader program
*/


class Texture{
public:
    unsigned int texture_id = 0;
    int width, height;

    Texture(const std::string& filename);

    void Bind(const int unit, const int programId, const std::string& name);
    void Unbind();
};
