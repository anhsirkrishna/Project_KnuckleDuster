#pragma once

class GameManager {
public:
	GameManager();
	void Quit();
	bool Status();
	unsigned int Level();
	void SetLevel(int level);
	void RestartLevel();
	int Debug();
	void SetDebug(bool _mode);
private:
	bool isRunning;
	int level;
	bool debug_mode;
};

extern GameManager* p_game_manager;