#pragma once

class GameManager {
public:
	GameManager();
	void Quit();
	bool Status();
	unsigned int Level();
	void SetLevel(int level);
private:
	bool isRunning;
	int level;
};

extern GameManager* p_game_manager;