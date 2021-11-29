#include "SDL_stdinc.h"

class InputManager {
public:
	InputManager();
	~InputManager();
	void Update();
	bool isKeyPressed(unsigned int keyScanCode);
	bool isKeyReleased(unsigned int keyScanCode);
	bool isKeyTriggered(unsigned int keyScanCode);
public:

private:
private:
	Uint8 mCurrentState[512];
	Uint8 mPrevState[512];
};

extern InputManager* pInputManager;
