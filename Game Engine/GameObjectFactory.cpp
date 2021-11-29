#include "GameObjectFactory.h"
#include <iostream>

GameObject* GameObjectFactory::CreateGameObject(std::string object_name, json components_json) {
	json component_data;
	std::string component_name;
	std::unordered_map<std::string, json> component_map;
	GameObject* new_object = new GameObject(object_name);
	component_map = components_json.get<std::unordered_map<std::string, json>>();
	for (std::pair<std::string, json> element : component_map) {
		component_name = element.first;
		component_data = element.second;
		new_object->AddComponent(
			component_name,
			component_factory.Create(component_name, component_data)
		);
	}
	new_object->LinkComponents();
	return new_object;
}

void GameObjectFactory::CreateLevel(unsigned int level) {
	std::string level_file = ".\\Levels\\Level_" + std::to_string(level) + ".json";
	std::ifstream level_data(level_file);

	json json_object;
	std::unordered_map<std::string, json> object_map;
	level_data >> json_object;

	object_map = json_object.get<std::unordered_map<std::string, json>>();

	for (std::pair<std::string, json> element : object_map) {
		pGameObjectManager->AddGameObject(CreateGameObject(element.first, element.second));
	}

	pGameObjectManager->RegisterObjectComponents();
}