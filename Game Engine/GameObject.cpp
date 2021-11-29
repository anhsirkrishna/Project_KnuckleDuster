#include "GameObject.h"
#include "ResourceManager.h"
#include "GLSprite.h"
#include "Transform.h"

GameObject::GameObject(std::string object_name) {
	name = object_name;
}

Component* GameObject::HasComponent(std::string component_name) const {
	try {
		return component_list.at(component_name);
	}// .at(key) throws an out_of_range exception if the key doesn't exist in the map
	catch (const std::out_of_range& oor) {
		return NULL;
	}
}

void GameObject::AddComponent(std::string component_name, Component* component) {
	Component* temp_component = HasComponent(component_name);
	if (temp_component != NULL)
		delete temp_component; //Overwrite existing component
	component_list.insert({ component_name, component });
	component->SetOwner(this);
}

void GameObject::RegisterObjectComponents(GameObject **game_object_list, unsigned int list_size, unsigned int self_index) {
	for (std::pair<std::string, Component*> element : component_list)
	{
		element.second->RegisterObjectComponents(game_object_list, list_size, self_index);
	}
}


void GameObject::Update() {
	for (std::pair<std::string, Component*> element : component_list)
	{
		element.second->Update();
	}
}

void GameObject::HandleInput() {
	for (std::pair<std::string, Component*> element : component_list)
	{
		element.second->HandleInput();
	}
}

void GameObject::HandleEvent(TimedEvent* p_event) {
	for (std::pair<std::string, Component*> element : component_list)
	{
		element.second->HandleEvent(p_event);
	}
}

void GameObject::LinkComponents() {
	for (std::pair<std::string, Component*> element : component_list)
	{
		element.second->Link();
	}
}

void GameObject::Draw(ShaderProgram* program) {
	for (std::pair<std::string, Component*> element : component_list)
	{
		if (element.first == "GLSPRITE") {
			GLSprite* glsprite_component = static_cast<GLSprite*>(element.second);
			glsprite_component->Draw(program);
		}
	}
}
