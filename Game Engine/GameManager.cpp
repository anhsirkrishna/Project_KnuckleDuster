#include "GameManager.h"

GameManager::GameManager() : isRunning(true) {
}

void GameManager::Quit() {
	isRunning = false;
}

bool GameManager::Status() {
	return isRunning;
}