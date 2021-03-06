#pragma once
#include "Component.h"

class Hurtbox;
class GLSprite;

enum class WalkDirection {
	None,
	Up,
	UpRight,
	Right,
	DownRight,
	Down,
	DownLeft,
	Left,
	UpLeft
};

class Walking : public Component {
public:
	Walking();
	void Serialize(json json_object);
	virtual void Update();
	virtual void Link();
	void Walk(WalkDirection dir);
	void StopWalk(WalkDirection dir);
	void SetDirXZero();
	void SetDirYZero();
private:
	int speed;
	int direction_x;
	int direction_y;
	Hurtbox* p_owner_hurtbox;
	GLSprite* p_owner_glsprite;
};