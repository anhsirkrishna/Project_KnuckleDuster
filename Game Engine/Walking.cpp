#include "Walking.h"
#include "GameObject.h"
#include "Hurtbox.h"
#include "GLSprite.h"

Walking::Walking() : Component("WALKING"), speed(0), direction_x(0), direction_y(0), p_owner_hurtbox(NULL), p_owner_glsprite(NULL) {}

void Walking::Serialize(json json_object) {
	speed = json_object["speed"].get<int>();
}
void Walking::Link() {	
	p_owner_hurtbox = static_cast<Hurtbox*>(GetOwner()->HasComponent("HURTBOX"));
	std::string current_state = GetOwner()->CurrentState();
	GetOwner()->ChangeState("WALK");
	p_owner_glsprite = static_cast<GLSprite*>(GetOwner()->HasComponent("GLSPRITE"));
	GetOwner()->ChangeState(current_state);
}

void Walking::Walk(WalkDirection dir) {
	switch (dir) {
	case WalkDirection::Up:
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
	if (direction_y == 1)
		p_owner_glsprite->SetTexture(1);//Texture that specfies moving down
	else if (direction_y == -1)
		p_owner_glsprite->SetTexture(2);//Texture that specfies moving up
	else
		p_owner_glsprite->SetTexture(0);//Texture that specfies moving right/left

	if (direction_x < 0)
		p_owner_hurtbox->SetScale(-1, 1);
	if (direction_x > 0)
		p_owner_hurtbox->SetScale(1, 1);

	if (direction_x == 0 && direction_y == 0)
		GetOwner()->ChangeState("IDLE");//No walking go back to idle state
	else
		GetOwner()->ChangeState("WALK");
}

void Walking::StopWalk(WalkDirection dir) {
	switch (dir) {
	case WalkDirection::Up:
		direction_y = (direction_y + 1) > 1 ? 1 : (direction_y + 1);
		break;
	case WalkDirection::Down:
		direction_y = (direction_y - 1) < -1 ? -1 : (direction_y - 1);
		break;
	case WalkDirection::Right:
		direction_x = (direction_x - 1) < -1 ? -1 : (direction_x - 1);
		break;
	case WalkDirection::Left:
		direction_x = (direction_x + 1) > 1 ? 1 : (direction_x + 1);
		break;
	}

	if (direction_y == 1)
		p_owner_glsprite->SetTexture(1);//Texture that specfies moving down
	else if (direction_y == -1)
		p_owner_glsprite->SetTexture(2);//Texture that specfies moving up
	else
		p_owner_glsprite->SetTexture(0);//Texture that specfies moving right/left

	if (direction_x < 0)
		p_owner_hurtbox->SetScale(-1, 1);
	if (direction_x > 0)
		p_owner_hurtbox->SetScale(1, 1);

	if (direction_x == 0 && direction_y == 0)
		GetOwner()->ChangeState("IDLE");//No walking go back to idle state
	else
		GetOwner()->ChangeState("WALK");
}

void Walking::SetDirXZero() {
	direction_x = 0;
}

void Walking::SetDirYZero() {
	direction_y = 0;
}

void Walking::Update() {
	if (GetOwner()->CurrentState() == "WALK") {
		SDL_Rect curr_position = p_owner_hurtbox->GetPosition();
		curr_position.x = curr_position.x + (speed * direction_x);
		curr_position.y = curr_position.y + (speed * direction_y);
		p_owner_hurtbox->SetPosition(curr_position);
	}
	else {
		direction_x = 0;
		direction_y = 0;
	}
}


