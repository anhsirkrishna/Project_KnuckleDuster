#pragma once

class ShaderProgram;

class BaseState {
public:
	virtual void Enter() {}
	virtual void Exit() {}
	virtual void Update() {}
	virtual void Render(ShaderProgram* p_program) {}
};