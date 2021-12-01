#include "GameObject.h"
#include "ResourceManager.h"
#include "GLSprite.h"
#include "Transform.h"

GameObject::GameObject(std::string object_name) : index(0) {
	name = object_name;
}

Component* GameObject::HasComponent(std::string component_name) const {

	for (auto component : component_list.at(current_state)) {
		if (component->GetName() == component_name)
			return component;
	}
	return NULL;
}

void GameObject::AddComponent(Component* component) {
	component_list.at(current_state).push_back(component);
	component->SetOwner(this);
}


void GameObject::Update() {
	for (auto component : component_list.at(current_state)) {
		component->Update();
	}
}

void GameObject::HandleInput() {
	for (auto component : component_list.at(current_state)) {
		component->HandleInput();
	}
}

void GameObject::HandleEvent(TimedEvent* p_event) {
	for (auto component : component_list.at(current_state)) {
		component->HandleEvent(p_event);
	}
}

void GameObject::LinkComponents() {
	for (auto component : component_list.at(current_state)) {
		component->Link();
	}
}

void GameObject::Draw(ShaderProgram* program) {
	for (auto component : component_list.at(current_state)) {
		component->Draw(program);
	}
}

void GameObject::AddState(std::string new_state) {
	component_list.insert(std::make_pair(new_state, std::vector<Component*>()));
}

void GameObject::ChangeState(std::string new_state) {
	current_state = new_state;
}