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

class KnockdownCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

class PlayerdazedCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

class JumpkickCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

class HostageCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

class EnemyAICreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};