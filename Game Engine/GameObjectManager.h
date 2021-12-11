#pragma once
#include "GameObject.h"

class GameObjectManager {
public:
	void AddGameObject(GameObject* game_object);
	void SetBackground(GameObject* game_object);
	void Update();
	void Draw(ShaderProgram* program);
	void Cleanup();
	void Delete(unsigned int index);
	const unsigned int max_objects = 50;
	GameObject* game_object_list[50];
	GameObject* p_background;
};

extern GameObjectManager* pGameObjectManager;