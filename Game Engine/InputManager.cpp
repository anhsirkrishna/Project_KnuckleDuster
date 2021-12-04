#include "InputManager.h"
#include "memory.h"
#include "SDL_keyboard.h"
#include "SDL_events.h"

InputManager::InputManager() {
	memset(mCurrentState, 0, 512* sizeof * mCurrentState);
	memset(mPrevState, 0, 512 * sizeof * mPrevState);
	prev_mouse_state = mouse_state = 0;
}
InputManager::~InputManager() {

}

bool InputManager::Update() {
	int numberOfItems = 0;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			return false;
		}
	}
	const Uint8*  currentKeyStates = SDL_GetKeyboardState(&numberOfItems);
	if (numberOfItems > 512)
		numberOfItems = 512;
	memcpy(mPrevState, mCurrentState, 512 * sizeof * mCurrentState);
	memcpy(mCurrentState, currentKeyStates, 512 * sizeof * mCurrentState);
	
	//Get mouse position
	prev_mouse_state = mouse_state;
	mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);


	if (isKeyTriggered(SDL_SCANCODE_ESCAPE)) {
		return false;
	}
	return true;
}

bool InputManager::isKeyPressed(unsigned int keyScanCode) {
	if (mCurrentState[keyScanCode])
		return true;
	
	return false;
}

bool InputManager::isKeyReleased(unsigned int keyScanCode) {
	if (mPrevState[keyScanCode] && (mCurrentState[keyScanCode] == 0))
		return true;

	return false;
}

bool InputManager::isKeyTriggered(unsigned int keyScanCode) {
	if (mPrevState[keyScanCode] == 0 && mCurrentState[keyScanCode])
		return true;

	return false;
}

bool InputManager::isMousePressed(unsigned int mouse_scan_code) {
	if ((mouse_state & mouse_scan_code) != 0)
		return true;

	return false;
}

bool InputManager::isMouseReleased(unsigned int mouse_scan_code) {
	if ((prev_mouse_state & mouse_scan_code) != 0 && ((mouse_state & mouse_scan_code) == 0))
		return true;

	return false;
}

bool InputManager::isMouseTriggered(unsigned int mouse_scan_code) {
	if (((prev_mouse_state & mouse_scan_code) == 0) && ((mouse_state & mouse_scan_code) != 0))
		return true;

	return false;
}