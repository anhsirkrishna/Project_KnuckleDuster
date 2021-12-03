#pragma once
#include "Component.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

class HealthCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};