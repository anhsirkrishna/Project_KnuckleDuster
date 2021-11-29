#pragma once

#include "ComponentCreator.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ComponentFactory {
public:
	ComponentFactory();
	~ComponentFactory();
	Component* Create(std::string component_name, json json_object);
private:
	std::unordered_map<std::string, ComponentCreator*> creator_map;
};