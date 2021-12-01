#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "ComponentFactory.h"

#include <fstream>

class GameObjectFactory {
public:
	GameObject* CreateGameObject(std::string object_name, std::string obj_def);
	void CreateLevel(unsigned int level);
private:
	ComponentFactory component_factory;
};