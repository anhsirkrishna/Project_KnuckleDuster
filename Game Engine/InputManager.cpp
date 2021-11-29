#include "InputManager.h"
#include "memory.h"
#include "SDL_keyboard.h"

InputManager::InputManager() {
	memset(mCurrentState, 0, 512* sizeof * mCurrentState);
	memset(mPrevState, 0, 512 * sizeof * mPrevState);
}
InputManager::~InputManager() {

}

void InputManager::Update() {
	int numberOfItems = 0;
	const Uint8*  currentKeyStates = SDL_GetKeyboardState(&numberOfItems);
	if (numberOfItems > 512)
		numberOfItems = 512;
	memcpy(mPrevState, mCurrentState, 512 * sizeof * mCurrentState);
	memcpy(mCurrentState, currentKeyStates, 512 * sizeof * mCurrentState);
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