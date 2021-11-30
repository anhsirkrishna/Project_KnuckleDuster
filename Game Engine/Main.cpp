#include <Windows.h>

#include <SDL.h>

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <SDL_image.h>

#include "Game.h"

#include "stdio.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "ResourceManager.h"
#include "GameObjectFactory.h"
#include "Matrix3D.h"
#include "EventManager.h"
#include "StateStackManager.h"
#include "StartState.h"
#include "GameManager.h"

FILE _iob[] = { *stdin, *stdout, *stderr };

unsigned int DEFAULT_FRAMERATE = 60;

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

////////////////////////////////////////////////////////////////////////
// This macro makes it easy to sprinkle checks for OpenGL errors
// throughout your code.  Most OpenGL calls can record errors, and a
// careful programmer will check the error status *often*, perhaps as
// often as after every OpenGL call.  At the very least, once per
// refresh will tell you if something is going wrong.
#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Main.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }


InputManager* pInputManager;
ResourceManager* pResourceManager;
GameObjectManager* pGameObjectManager;
FrameRateController* pFrameRateController;
EventManager* p_event_manager;
StateStackManager* p_statestack_manager;
GameManager* p_game_manager;

unsigned int WORLD_WIDTH = 720;
unsigned int WORLD_HEIGHT = 480;

unsigned int WINDOW_WIDTH = 720;
unsigned int WINDOW_HEIGHT = 480;

//Global SDL vars
SDL_Window* pWindow;
//OpenGL context
SDL_GLContext gContext;

//Create all the necessary Global Manager objects
void CreateManagers() {
	pInputManager = new InputManager;
	pFrameRateController = new FrameRateController(DEFAULT_FRAMERATE);
	pResourceManager = new ResourceManager();
	pGameObjectManager = new GameObjectManager();
	p_event_manager = new EventManager();
	p_statestack_manager = new StateStackManager();
	p_game_manager = new GameManager();
}

void DeleteManagers() {
	delete pInputManager;
	delete pResourceManager;
	delete pFrameRateController;
	pGameObjectManager->Cleanup();
	delete pGameObjectManager;
	delete p_statestack_manager;
	delete p_game_manager;
}

//Function to initalize SDL and Open GL 
bool SDL_GL_Init() {

	int error = 0;
	// Initialize SDL
	if ((error = SDL_Init(SDL_INIT_VIDEO)) < 0)
	{
		SDL_Log("Couldn't initialize SDL, error %i\n", error);
		return false;
	}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	pWindow = SDL_CreateWindow("SDL2 window",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		WINDOW_WIDTH,								// width, in pixels
		WINDOW_HEIGHT,								// height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	//Create context
	gContext = SDL_GL_CreateContext(pWindow);
	if (gContext == NULL)
	{
		SDL_Log("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			SDL_Log("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
			return false;
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			SDL_Log("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}
	}

	return true;
}

void CloseProgram() {
	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();
	FreeConsole();
}

ShaderProgram* GL_Program_init() {
	ShaderProgram *p_shader_program = new ShaderProgram();
	p_shader_program->AddShader("final.vert", GL_VERTEX_SHADER);
	p_shader_program->AddShader("final.frag", GL_FRAGMENT_SHADER);

	glBindAttribLocation(p_shader_program->program_id, 0, "in_position");
	glBindAttribLocation(p_shader_program->program_id, 1, "in_Color");
	glBindAttribLocation(p_shader_program->program_id, 2, "in_TexCoords");
	p_shader_program->LinkProgram();
	CHECKERROR;

	return p_shader_program;
}

int main(int argc, char* args[])
{
	AllocConsole();
	bool appIsRunning = true;
	int mode = 1;
	unsigned short curr_level = 1;
	unsigned int window_width = WORLD_WIDTH;
	unsigned int window_height = WORLD_HEIGHT;

	CreateManagers();

	GameObjectFactory obj_factory;

	if (SDL_GL_Init())
		SDL_Log("Initialization Complete");
	else {
		SDL_Log("Initialization Failed");
		return 1;
	}

	ShaderProgram *p_shader_program = GL_Program_init();
	//Load resources using the resource manager

	p_statestack_manager->Push(new StartState());

	// Game loop
	while(p_game_manager->Status())
	{
		pFrameRateController->start_game_loop();

		p_statestack_manager->Update();

		p_statestack_manager->Render(p_shader_program);
		
		SDL_GL_SwapWindow(pWindow);
		pFrameRateController->end_game_loop();
	}

	DeleteManagers();
	CloseProgram();
	return 0;
}