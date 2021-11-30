#include "Component.h"
#include "GameObject.h"

Component::Component(std::string _name) : name(_name), p_owner_object(NULL) {}

std::string Component::GetName() {
	return name;
}

void Component::SetOwner(GameObject* owner_object) {
	p_owner_object = owner_object;
}

GameObject* Component::GetOwner() {
	return p_owner_object;
}

void Component::RegisterObjectComponents(GameObject **game_object_list, unsigned int list_size, unsigned int self_index) {

}

void Component::Update() {

}

void Component::HandleInput() {

}

void Component::HandleEvent(TimedEvent* p_event) {

}

void Component::Link() {

}

void Component::Draw(ShaderProgram* p_program) {

}