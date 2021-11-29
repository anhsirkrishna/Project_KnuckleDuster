#include "GameManager.h"

GameManager::GameManager() : isRunning(true), level(0) {
}

void GameManager::Quit() {
	isRunning = false;
}

bool GameManager::Status() {
	return isRunning;
}

unsigned int GameManager::Level() {
	return level;
}

void GameManager::SetLevel(unsigned int next_level) {
	level = next_level;
}