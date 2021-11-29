#pragma once
#include <vector> 

class BaseState;
class ShaderProgram;

class StateStackManager {
public:
	void Update();
	void Render(ShaderProgram *p_program);
	void Clear();
	void Push(BaseState* p_state);
	void Pop();
	~StateStackManager();
private:
	std::vector<BaseState*> state_stack;
	std::vector<BaseState*> to_delete;
};

extern StateStackManager* p_statestack_manager;