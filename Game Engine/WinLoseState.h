#pragma once
#include "BaseState.h"
#include <vector>

class GameObject;

class WinLoseState : public BaseState {
public:
	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Render(ShaderProgram* p_program);
private:
	std::vector<GameObject*> game_object_list;
};