#include "SDL_stdinc.h"

class InputManager {
public:
	InputManager();
	~InputManager();
	bool Update();
	bool isKeyPressed(unsigned int keyScanCode);
	bool isKeyReleased(unsigned int keyScanCode);
	bool isKeyTriggered(unsigned int keyScanCode);
	bool isMouseTriggered(unsigned int mouseScanCode);
	bool isMouseReleased(unsigned int mouseScanCode);
	bool isMousePressed(unsigned int mouseScanCode);
public:
	int mouse_x, mouse_y;
private:
private:
	Uint8 mCurrentState[512];
	Uint8 mPrevState[512];
	Uint32 mouse_state;
	Uint32 prev_mouse_state;
};

extern InputManager* pInputManager;
