#pragma once

#include "SDL.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class GameObject;
class TimedEvent;
class ShaderProgram;

class Component {
public:
	Component(std::string _name);
	void SetOwner(GameObject* p_owner_object);
	GameObject* GetOwner();
	virtual void Update();
	virtual void HandleInput();
	virtual void HandleEvent(TimedEvent* p_event);
	virtual void RegisterObjectComponents(GameObject **game_object_list,unsigned int list_size,unsigned int self_index);
	virtual void Link();
	virtual void Draw(ShaderProgram* p_program);
	std::string GetName();
private:
	std::string name;
	GameObject* p_owner_object;
};
