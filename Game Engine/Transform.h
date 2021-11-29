#pragma once

#include "Component.h"
#include "SDL_rect.h"
#include "Matrix3D.h"

class Transform : public Component {
public:
	SDL_Rect GetPosition();
	float GetRotation();
	void SetPosition(SDL_Rect new_position);
	void SetRotation(float new_rotation);
	void Serialize(json json_object);
	void Update();
	Matrix3D GetTranslateMatrix();
	Matrix3D GetRotateMatrix();
	Matrix3D GetPreRotateMatrix();
	Matrix3D GetPostRotateMatrix();
	void SetRotMatrices();
private:
	SDL_Rect position;
	Matrix3D translate_matrix;
	Matrix3D rotate_matrix;
	Matrix3D pre_rotate_matrix;
	Matrix3D post_rotate_matrix;
	float rotation = 0;
};