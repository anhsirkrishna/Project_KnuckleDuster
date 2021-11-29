#pragma once

#include <unordered_map>
#include <stdexcept>

#include "Component.h"
#include "ShaderProgram.h"

class TimedEvent;

class GameObject {
	public:
		GameObject(std::string object_name);
		Component* HasComponent(std::string component_name) const;
		void AddComponent(std::string component_name, Component* component);
		void Update();
		void HandleInput();
		void HandleEvent(TimedEvent* p_event);
		void RegisterObjectComponents(GameObject **game_object_list, unsigned int list_size, unsigned int self_index);
		void LinkComponents();
		void Draw(ShaderProgram* program);
	private:
		std::unordered_map<std::string, Component*> component_list;
		std::string name;
};