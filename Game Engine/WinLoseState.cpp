#include "WinLoseState.h"
#include "GameObject.h"
#include "Panel.h"
#include "Textbox.h"
#include "Button.h"
#include "GameManager.h"
#include "Game.h"
#include "InputManager.h"
#include "StateStackManager.h"
#include "GameState.h"

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Main.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }

void ButtonFuncQuit() {
	p_game_manager->Quit();
}

void ButtonFuncRestart() {
	p_game_manager->RestartLevel();

}

void WinLoseState::Enter() {
	GameObject* ui_obj = new GameObject("PauseMenu");
	ui_obj->AddState("DEFAULT");
	ui_obj->ChangeState("DEFAULT");
	ui_obj->AddComponent(new Panel(200, 120, 20, 18));
	std::string textbox_text;
	if (p_game_manager->Level() == -1)
		textbox_text = "GAME OVER";
	else
		textbox_text = "  YOU WIN";

	ui_obj->AddComponent(new Textbox(240, 140, textbox_text.size(), 1, textbox_text, 2.0));
	textbox_text = "QUIT";
	ui_obj->AddComponent(new Button(250, 260, 12, textbox_text, &ButtonFuncQuit));
	textbox_text = "RESTART";
	ui_obj->AddComponent(new Button(250, 200, 12, textbox_text, &ButtonFuncRestart));
	game_object_list.push_back(ui_obj);
}

void WinLoseState::Update() {
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

	if (p_game_manager->Level() == 0) {
		p_statestack_manager->Pop();
		return;
	}
}

void WinLoseState::Render(ShaderProgram* p_program) {
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

void WinLoseState::Exit() {
	for (auto game_obj : game_object_list) {
		delete game_obj;
	}
}