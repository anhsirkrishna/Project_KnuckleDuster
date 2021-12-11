#pragma once
#include "BaseState.h"

class StartState : public BaseState {
	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Render(ShaderProgram* p_program);
private:
	bool show_instructions;
};