#pragma once
#include "GameObject.h"

class GameObjectManager {
public:
	void AddGameObject(GameObject* game_object);
	void RegisterObjectComponents();
	void Update();
	void Draw(ShaderProgram* program);
	void Cleanup();
	const unsigned int max_objects = 50;
	GameObject* game_object_list[50];
};

extern GameObjectManager* pGameObjectManager;