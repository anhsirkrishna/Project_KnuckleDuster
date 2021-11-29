#include "StateStackManager.h"
#include "BaseState.h"

void StateStackManager::Update() {
	state_stack.back()->Update();
}

void StateStackManager::Render(ShaderProgram* p_program) {
	for (auto state : state_stack) {
		state->Render(p_program);
	}
}
void StateStackManager::Clear() {
	while (to_delete.size() > 0)
	{
		delete to_delete.back();
		to_delete.pop_back();
	}
	while (state_stack.size() > 0)
	{
		delete state_stack.back();
		state_stack.pop_back();
	}
}

void StateStackManager::Push(BaseState* p_state) {
	state_stack.push_back(p_state);
	state_stack.back()->Enter();
}

void StateStackManager::Pop() {
	state_stack.back()->Exit();
	to_delete.push_back(state_stack.back());
	state_stack.pop_back();
}

StateStackManager::~StateStackManager() {
	Clear();
}