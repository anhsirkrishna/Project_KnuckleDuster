#pragma once

#include "Component.h"
#include "Transform.h"
#include "InputManager.h"
#include "Util.h"

class Walking;
class Punching;
class Jumpkick;

class Controller : public Component {
public:
	Controller();
	virtual void HandleInput();
	virtual void Link();
	void Serialize(json json_object);
private:
	Walking* p_owner_walking;
	Punching* p_owner_punching;
	Jumpkick* p_owner_jumpkick;
	RingBuffer<Uint8, 32> input_buffer;
};