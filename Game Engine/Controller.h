#pragma once

#include "Component.h"
#include "Transform.h"
#include "InputManager.h"

class Walking;
class Punching;

class Controller : public Component {
public:
	Controller();
	virtual void HandleInput();
	virtual void Link();
	void Serialize(json json_object);
private:
	Walking* p_owner_walking;
	Punching* p_owner_punching;
};