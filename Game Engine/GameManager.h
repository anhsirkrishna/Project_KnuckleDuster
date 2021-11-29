#pragma once

class GameManager {
public:
	GameManager();
	void Quit();
	bool Status();
	unsigned int Level();
	void SetLevel(unsigned int level);
private:
	bool isRunning;
	unsigned int level;
};

extern GameManager* p_game_manager;