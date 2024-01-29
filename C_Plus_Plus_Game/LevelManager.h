#pragma once
#include "Manager.h"
#include <vector>
#include <string>

class LevelManager: public Manager<LevelManager>
{
	class GameState* m_state;
	bool m_win = false;
public:
	int m_level_counter = -1; // starts counting from 0
	std::vector<std::string> levels_list;
	bool m_restart = false;
	void init();
	void update(float dt);
	void nextLevel(bool restartLevel = false);
	void restartLevel();
	void onPlayerDied();
	void saveData();
	void loadSaveFile();
	bool m_loadingFile = false;
};