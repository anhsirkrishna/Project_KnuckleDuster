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
	float GetScaleX();
	float GetScaleY();
	void SetPosition(SDL_Rect new_position);
	void SetScale(float _scale_x, float _scale_y);
	bool CheckOutOfBounds();
	bool CheckCollision();
private:
	SDL_Rect box;
	int scale_x, scale_y;
	Transform* p_owner_transform;
};