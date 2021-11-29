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

unsigned int WORLD_WIDTH = 900;
unsigned int WORLD_HEIGHT = 600;

int main(int argc, char* args[])
{
	SDL_Window *pWindow;
	//OpenGL context
	SDL_GLContext gContext;

	AllocConsole();
	int error = 0;
	bool appIsRunning = true;
	int mode = 1;
	unsigned short  curr_level = 1;
	SDL_Surface* pWindowSurface = NULL;
	SDL_Surface* ppImageSurfaces[3] = { NULL, NULL, NULL};

	pInputManager = new InputManager;
	pFrameRateController = new FrameRateController(DEFAULT_FRAMERATE);
	pResourceManager = new ResourceManager();
	pGameObjectManager = new GameObjectManager();
	p_event_manager = new EventManager();

	GameObjectFactory obj_factory;

	SDL_Rect charRect;
	charRect.x = 400;
	charRect.y = 300;
	charRect.h = 96;
	charRect.w = 84;
	char* orientation[4] = { "Up", "Down", "Right", "Left" };
	int currOrientation=0;

	// Initialize SDL
	if((error = SDL_Init( SDL_INIT_VIDEO )) < 0 )
	{
		SDL_Log("Couldn't initialize SDL, error %i\n", error);
		return 1;
	}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	unsigned int window_width = WORLD_WIDTH;
	unsigned int window_height = WORLD_HEIGHT;

	pWindow = SDL_CreateWindow("SDL2 window",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		window_width,								// width, in pixels
		window_height,								// height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	
	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	//Create context
	gContext = SDL_GL_CreateContext(pWindow);
	if (gContext == NULL)
	{
		SDL_Log("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			SDL_Log("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			SDL_Log("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		}
	}

	ShaderProgram shader_program;
	shader_program.AddShader("final.vert", GL_VERTEX_SHADER);
	shader_program.AddShader("final.frag", GL_FRAGMENT_SHADER);

	glBindAttribLocation(shader_program.program_id, 0, "in_position");
	glBindAttribLocation(shader_program.program_id, 1, "in_Color");
	glBindAttribLocation(shader_program.program_id, 2, "in_TexCoords");
	shader_program.LinkProgram();
	
	CHECKERROR;
	//Load resources using the resource manager

	obj_factory.CreateLevel(curr_level);

	// Game loop
	while(true == appIsRunning)
	{
		pFrameRateController->start_game_loop();
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				appIsRunning = false;
			}
		}
		//const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		pInputManager->Update();

		if (pInputManager->isKeyPressed(SDL_SCANCODE_ESCAPE)) {
			appIsRunning = false;
		}

		if (pInputManager->isKeyTriggered(SDL_SCANCODE_SPACE)) {
			mode = (mode + 1) % 2;
		}

		shader_program.Use();
		glClearColor(0.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Matrix3D orthoGraphProj = OrthographicProj(0, window_width, 0, window_height, 0, 1.0);
		GLuint loc = glGetUniformLocation(shader_program.program_id, "orthoGraphProj");
		glUniformMatrix4fv(loc, 1, GL_FALSE, orthoGraphProj.GetMatrixP());
		
		loc = glGetUniformLocation(shader_program.program_id , "mode");
		glUniform1i(loc, mode);

		pGameObjectManager->Update();
		p_event_manager->Update();
		CHECKERROR;
		//Redraw the scene every frame
		//SDL_BlitSurface(pResourceManager->get_resource("Background"), NULL, pWindowSurface, NULL);
		pGameObjectManager->Draw(&shader_program);
		//SDL_UpdateWindowSurface(pWindow);
		CHECKERROR;
		SDL_GL_SwapWindow(pWindow);
		//Limit the framerate
		shader_program.Unuse();
		pFrameRateController->end_game_loop();
	}

	delete pInputManager;
	delete pResourceManager;
	delete pFrameRateController;
	pGameObjectManager->Cleanup();
	delete pGameObjectManager;

	SDL_FreeSurface(pWindowSurface);

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();
	FreeConsole();
	return 0;
}