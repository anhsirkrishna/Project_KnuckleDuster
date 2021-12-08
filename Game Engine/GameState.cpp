#include "GameState.h"
#include "GameObjectFactory.h"
#include "EventManager.h"
#include "InputManager.h"
#include "GameManager.h"
#include "Matrix3D.h"
#include "Game.h"
#include "StateStackManager.h"
#include "PauseState.h"
#include "LoseState.h"

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Main.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }

void GameState::Enter() {
	GameObjectFactory obj_factory;
	obj_factory.CreateLevel(p_game_manager->Level());
}

void GameState::Update() {
	if (p_game_manager->Level() == 0) {
		p_statestack_manager->Pop();
		p_game_manager->SetLevel(curr_level);
		p_statestack_manager->Push(new GameState());
	}

	curr_level = p_game_manager->Level();
	if (pInputManager->Update() == false) {
		p_statestack_manager->Push(new PauseState());
	}
	pGameObjectManager->Update();
	p_event_manager->Update();

	if (p_game_manager->Level() == -1) {
		p_statestack_manager->Push(new LoseState());
	}
	else if (p_game_manager->Level() != curr_level) {
		p_statestack_manager->Push(new GameState());
	}
}

void GameState::Render(ShaderProgram* p_program) {
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

void GameState::Exit() {
	pGameObjectManager->Cleanup();
}