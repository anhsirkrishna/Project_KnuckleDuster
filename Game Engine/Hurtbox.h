#pragma once

#include "Component.h"
#include "Transform.h"


class Hurtbox : public Component {
public:
	Hurtbox();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	virtual void HandleEvent(TimedEvent* p_event);
	SDL_Rect GetPosition();
	void SetPosition(SDL_Rect new_position);
	bool CheckOutOfBounds();
	bool CheckCollision();
private:
	SDL_Rect box;
	Transform* p_owner_transform;
	std::vector<Hurtbox*> hurtbox_list;
};