#include "GameObjectFactory.h"
#include <iostream>

GameObject* GameObjectFactory::CreateGameObject(std::string object_name, std::string obj_def) {
	GameObject* new_object = new GameObject(object_name);
	
	std::string obj_def_file = ".\\Obj_defs\\" + obj_def + ".json";
	std::ifstream obj_def_data(obj_def_file);

	json json_object;
	obj_def_data >> json_object;
	auto state_map = json_object.get<std::unordered_map<std::string, json>>();

	json component_data;
	std::string component_name;
	std::string state_name;
	for (auto state : state_map) {
		state_name = state.first;
		new_object->AddState(state_name);
		new_object->ChangeState(state_name);
		auto component_map = state.second.get<std::unordered_map<std::string, json>>();
		for (auto element : component_map) {
			component_name = element.first;
			component_data = element.second;
			new_object->AddComponent(
				component_factory.Create(component_name, component_data)
			);
		}
		new_object->LinkComponents();
	}

	return new_object;
}

void GameObjectFactory::CreateLevel(unsigned int level) {
	std::string level_file = ".\\Levels\\Level_" + std::to_string(level) + ".json";
	std::ifstream level_data(level_file);

	json json_object;
	level_data >> json_object;

	auto object_map = json_object.get<std::unordered_map<std::string, json>>();

	GameObject* new_object;
	std::string obj_def;
	std::string default_state;
	for (auto element : object_map) {
		obj_def = element.second["obj_def"].get<std::string>();
		default_state = element.second["default_state"].get<std::string>();
		new_object = CreateGameObject(element.first, obj_def);
		new_object->ChangeState(default_state);
		pGameObjectManager->AddGameObject(new_object);
	}
}