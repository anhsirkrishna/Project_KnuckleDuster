#include "GameObjectManager.h"

void GameObjectManager::AddGameObject(GameObject* game_object) {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL) {
			game_object_list[i] = game_object;
			return;
		}
	}
}

void GameObjectManager::RegisterObjectComponents() {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL) {
			continue;
		}
		game_object_list[i]->RegisterObjectComponents(game_object_list, max_objects, i);
	}
}

void GameObjectManager::Update() {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;

		game_object_list[i]->HandleInput();
	}

	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;
		
		game_object_list[i]->Update();
	}
}

void GameObjectManager::Draw(ShaderProgram* program) {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;

		game_object_list[i]->Draw(program);
	}
}

void GameObjectManager::Cleanup() {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;

		delete game_object_list[i];
	}
}