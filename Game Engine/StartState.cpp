#include "StartState.h"
#include "GameObjectFactory.h"
#include "EventManager.h"
#include "InputManager.h"
#include "GameManager.h"
#include "Matrix3D.h"
#include "Game.h"
#include "StateStackManager.h"
#include "GameState.h"
#include "GameObjectManager.h"
#include "Panel.h"
#include "Textbox.h"

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Main.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }

void StartState::Enter() {
	GameObjectFactory obj_factory;
	obj_factory.CreateLevel(0);

	GameObject *ui_obj = new GameObject("UI_panel");
	ui_obj->AddState("DEFAULT");
	ui_obj->ChangeState("DEFAULT");
	ui_obj->AddComponent(new Panel(170, 240, 25, 4));
	std::string textbox_text = "PRESS ENTER TO START";
	ui_obj->AddComponent(new Textbox(188, 252, textbox_text.size(), 1, textbox_text, 2.0));
	pGameObjectManager->AddGameObject(ui_obj);
}

void StartState::Update() {
	if (pInputManager->Update() == false) {
		p_game_manager->Quit();
		return;
	}

	if (pInputManager->isKeyTriggered(SDL_SCANCODE_RETURN)) {
		p_statestack_manager->Pop();
		p_game_manager->SetLevel(1);
		p_statestack_manager->Push(new GameState());
		return;
	}

	pGameObjectManager->Update();
	p_event_manager->Update();
}

void StartState::Render(ShaderProgram* p_program) {
	p_program->Use();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix3D orthoGraphProj = OrthographicProj(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1.0);
	GLuint loc = glGetUniformLocation(p_program->program_id, "orthoGraphProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, orthoGraphProj.GetMatrixP());

	loc = glGetUniformLocation(p_program->program_id, "mode");
	glUniform1i(loc, 1);

	CHECKERROR;
	//Redraw the scene every frame
	pGameObjectManager->Draw(p_program);
	CHECKERROR;
	p_program->Unuse();
}

void StartState::Exit() {
	pGameObjectManager->Cleanup();
}