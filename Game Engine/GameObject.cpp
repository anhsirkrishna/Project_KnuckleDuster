#include "GameObject.h"
#include "ResourceManager.h"
#include "GLSprite.h"
#include "Transform.h"

GameObject::GameObject(std::string object_name) {
	name = object_name;
}

Component* GameObject::HasComponent(std::string component_name) const {

	for (auto component : component_list) {
		if (component->GetName() == component_name)
			return component;
	}
	return NULL;
}

void GameObject::AddComponent(Component* component) {
	component_list.push_back(component);
	component->SetOwner(this);
}

void GameObject::RegisterObjectComponents(GameObject **game_object_list, unsigned int list_size, unsigned int self_index) {
	for (auto component : component_list) {
		component->RegisterObjectComponents(game_object_list, list_size, self_index);
	}
}


void GameObject::Update() {
	for (auto component : component_list) {
		component->Update();
	}
}

void GameObject::HandleInput() {
	for (auto component : component_list) {
		component->HandleInput();
	}
}

void GameObject::HandleEvent(TimedEvent* p_event) {
	for (auto component : component_list) {
		component->HandleEvent(p_event);
	}
}

void GameObject::LinkComponents() {
	for (auto component : component_list) {
		component->Link();
	}
}

void GameObject::Draw(ShaderProgram* program) {
	for (auto component : component_list) {
		component->Draw(program);
	}
}
