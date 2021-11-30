#pragma once

#include "Component.h"
#include "Transform.h"
#include "InputManager.h"

class Hurtbox;

class Controller : public Component {
public:
	Controller();
	virtual void HandleInput();
	virtual void Link();
	void Serialize(json json_object);
private:
	Hurtbox* p_owner_hurtbox;
};