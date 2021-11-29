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
	state_stack.clear();
}

void StateStackManager::Push(BaseState* p_state) {
	state_stack.push_back(p_state);
	state_stack.back()->Enter();
}

void StateStackManager::Pop() {
	state_stack.back()->Exit();
	delete state_stack.back();
	state_stack.pop_back();
}