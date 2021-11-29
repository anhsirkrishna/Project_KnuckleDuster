#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "SDL.h"
#include "Texture.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	/*Function to load the resource if it's not already loaded.*/
	void add_resource(std::string file_name);
	/*Function to free all the loaded resources.*/
	void free_resources();
	/*Function that returns the pointer to the resource identified by the name.
	  Returns NULL if the resources isn't loaded.*/
	SDL_Surface* get_resource(std::string file_name);

	void add_texture(std::string file_name);
	Texture* get_texture(std::string name);
public:
private:
private:
	std::unordered_map<std::string, SDL_Surface*> m_resource_map;
	std::unordered_map<std::string, Texture*> m_textures_map;
};

extern ResourceManager* pResourceManager;