#pragma once

#include <unordered_map>
#include <stdexcept>

#include "Component.h"
#include "ShaderProgram.h"

class TimedEvent;

class GameObject {
	public:
		GameObject(std::string object_name);
		std::string GameObject::GetName();
		Component* HasComponent(std::string component_name) const;
		void AddComponent(Component* component);
		void AddComponentToState(std::string, Component* component);
		void Update();
		void HandleInput();
		void HandleEvent(TimedEvent* p_event);
		void LinkComponents();
		void Draw(ShaderProgram* program);
		void AddState(std::string new_state);
		void ChangeState(std::string new_state);
		std::string CurrentState();
		unsigned int index;
	private:
		std::string name;
		std::vector<Component*> component_list;
		std::unordered_map<std::string, std::vector<Component*>> state_component_list;
		std::string current_state;
};