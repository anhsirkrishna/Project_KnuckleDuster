#pragma once
#include "BaseState.h"

class GameState : public BaseState {
	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Render(ShaderProgram* p_program);
};