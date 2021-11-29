#pragma once

class GameManager {
public:
	GameManager();
	void Quit();
	bool Status();
private:
	bool isRunning;
};

extern GameManager* p_game_manager;