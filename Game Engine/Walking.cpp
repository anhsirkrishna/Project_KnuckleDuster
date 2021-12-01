#include "Walking.h"
#include "GameObject.h"
#include "Hurtbox.h"

Walking::Walking() : Component("WALKING"), speed(0), direction_x(0), direction_y(0), p_owner_hurtbox(0) {}

void Walking::Serialize(json json_object) {
	speed = json_object["speed"].get<int>();
}
void Walking::Link() {	
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
}

void Walking::Walk(WalkDirection dir) {
	switch (dir) {
	case WalkDirection::Up :
		direction_y = (direction_y - 1) < -1 ? -1 : (direction_y - 1);
		break;
	case WalkDirection::Down:
		direction_y = (direction_y + 1) > 1 ? 1 : (direction_y + 1);
		break;
	case WalkDirection::Right:
		direction_x = (direction_x + 1) > 1 ? 1 : (direction_x + 1);
		break;
	case WalkDirection::Left:
		direction_x = (direction_x - 1) < -1 ? -1 : (direction_x - 1);
		break;
	}
	GetOwner()->ChangeState("WALK");
}

void Walking::StopWalk(WalkDirection dir) {
	switch (dir) {
	case WalkDirection::Up:
		direction_y = 0;
		break;
	case WalkDirection::Down:
		direction_y = 0;
		break;
	case WalkDirection::Right:
		direction_x = 0;
		break;
	case WalkDirection::Left:
		direction_x = 0;
		break;
	}
}

void Walking::Update() {
	if (direction_x == 0 && direction_y == 0) {
		//No walking go back to idle state
		GetOwner()->ChangeState("IDLE");
		return;
	}

	SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
	curr_position.x = curr_position.x + (speed * direction_x);
	curr_position.y = curr_position.y + (speed * direction_y);
	p_owner_hurtbox->SetPosition(curr_position);
	float scale_x = direction_x < 0 ? -1 : 1;
	p_owner_hurtbox->SetScale(scale_x, 1);
}


