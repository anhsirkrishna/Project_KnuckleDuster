#include "GameManager.h"

GameManager::GameManager() : isRunning(true), level(0), debug_mode(false) {
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

void GameManager::SetLevel(int next_level) {
	level = next_level;
}

void GameManager::RestartLevel() {
	level = 0;
}

void GameManager::SetDebug(bool _mode) {
	debug_mode = _mode;
}

int GameManager::Debug() {
	if (debug_mode)
		return 0;
	else
		return 1;
}