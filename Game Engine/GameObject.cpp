#include "GameObject.h"
#include "ResourceManager.h"
#include "GLSprite.h"
#include "Transform.h"

GameObject::GameObject(std::string object_name) : index(0) {
	name = object_name;
	current_state;
}

std::string GameObject::GetName() {
	return name;
}

Component* GameObject::HasComponent(std::string component_name) const {
	for (auto component : component_list) {
		if (component->GetName() == component_name)
			return component;
	}
	if (current_state.empty())
		return NULL;

	for (auto component : state_component_list.at(current_state)) {
		if (component->GetName() == component_name)
			return component;
	}
	return NULL;
}

void GameObject::AddComponent(Component* component) {
	component_list.push_back(component);
	component->SetOwner(this);
}

void GameObject::AddComponentToState(std::string, Component* component) {
	state_component_list.at(current_state).push_back(component);
	component->SetOwner(this);
}

void GameObject::Update() {
	for (auto component : component_list) {
		component->Update();
	}
	if (current_state.empty())
		return;
	for (auto component : state_component_list.at(current_state)) {
		component->Update();
	}
}

void GameObject::HandleInput() {
	for (auto component : component_list) {
		component->HandleInput();
	}
	if (current_state.empty())
		return;
	for (auto component : state_component_list.at(current_state)) {
		component->HandleInput();
	}
}

void GameObject::HandleEvent(TimedEvent* p_event) {
	for (auto component : component_list) {
		component->HandleEvent(p_event);
	}
	if (current_state.empty())
		return;
	for (auto component : state_component_list.at(current_state)) {
		component->HandleEvent(p_event);
	}
}

void GameObject::LinkComponents() {
	for (auto component : component_list) {
		component->Link();
	}
	for (auto component_list : state_component_list) {
		ChangeState(component_list.first);
		for (auto component : component_list.second)
			component->Link();
	}
}

void GameObject::Draw(ShaderProgram* program) {
	for (auto component : component_list) {
		component->Draw(program);
	}
	if (current_state.empty())
		return;
	for (auto component : state_component_list.at(current_state)) {
		component->Draw(program);
	}
}

void GameObject::AddState(std::string new_state) {
	state_component_list.insert(std::make_pair(new_state, std::vector<Component*>()));
}

void GameObject::ChangeState(std::string new_state) {
	current_state = new_state;
}
std::string GameObject::CurrentState() {
	return current_state;
}