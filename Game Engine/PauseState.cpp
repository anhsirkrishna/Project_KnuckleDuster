#include "PauseState.h"
#include "InputManager.h"
#include "GameManager.h"
#include "Matrix3D.h"
#include "Game.h"
#include "StateStackManager.h"
#include "Panel.h"
#include "Textbox.h"
#include "Button.h"
#include "GameObject.h"

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Main.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }

void ButtonFunc() {
	p_game_manager->Quit();
}

void DebugToggle() {
	if (p_game_manager->Debug() == 0) {
		p_game_manager->SetDebug(false);
	}
	else if (p_game_manager->Debug() == 1) {
		p_game_manager->SetDebug(true);
	}
}

void PauseState::Enter() {
	GameObject* ui_obj = new GameObject("PauseMenu");
	ui_obj->AddState("DEFAULT");
	ui_obj->ChangeState("DEFAULT");
	ui_obj->AddComponent(new Panel(200, 120, 20, 18));
	std::string textbox_text = "GAME PAUSED";
	ui_obj->AddComponent(new Textbox(230, 140, textbox_text.size(), 1, textbox_text, 2.0));
	textbox_text = "DEBUG TOGGLE";
	ui_obj->AddComponent(new Button(250, 200, 12, textbox_text, &DebugToggle));
	textbox_text = "QUIT";
	ui_obj->AddComponent(new Button(250, 300, 12, textbox_text, &ButtonFunc));
	game_object_list.push_back(ui_obj);
}

void PauseState::Update() {
	if (pInputManager->Update() == false) {
		p_statestack_manager->Pop();
		return;
	}

	if (pInputManager->isKeyTriggered(SDL_SCANCODE_RETURN)) {
		p_statestack_manager->Pop();
		return;
	}

	for (auto game_obj : game_object_list) {
		game_obj->Update();
	}
}

void PauseState::Render(ShaderProgram* p_program) {
	p_program->Use();

	Matrix3D orthoGraphProj = OrthographicProj(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1.0);
	GLuint loc = glGetUniformLocation(p_program->program_id, "orthoGraphProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, orthoGraphProj.GetMatrixP());

	loc = glGetUniformLocation(p_program->program_id, "mode");
	glUniform1i(loc, 1);

	CHECKERROR;
	//Redraw the scene every frame
	for (auto game_obj : game_object_list) {
		game_obj->Draw(p_program);
	}
	CHECKERROR;
	p_program->Unuse();
}

void PauseState::Exit() {
	for (auto game_obj : game_object_list) {
		delete game_obj;
	}
}