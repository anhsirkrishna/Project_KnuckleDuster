#pragma once
#include "BaseState.h"
#include <vector>

class EnemySpawner;

class GameState : public BaseState {
	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Render(ShaderProgram* p_program);
private:
	int curr_level;
	std::vector<EnemySpawner*> spawners;
};