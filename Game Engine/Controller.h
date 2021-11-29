#pragma once

#include "Component.h"
#include "Transform.h"
#include "InputManager.h"

class Hurtbox;

class Controller : public Component {
public:
	virtual void HandleInput();
	virtual void Link();
	void Serialize(json json_object);
private:
	unsigned short direction;
	Hurtbox* p_owner_hurtbox;
};