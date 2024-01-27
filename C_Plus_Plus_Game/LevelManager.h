#pragma once
#include <vector>
#include <string>

class LevelManager
{
	class GameState* m_state;
	static LevelManager* s_unique_instance;
	bool m_win = false;
public:
	int m_level_counter = -1; // starts counting from 0
	std::vector<std::string> levels_list;
	bool m_restart = false;
	void init();
	void update(float dt);
	static LevelManager* getInstance();
	void nextLevel(bool restartLevel = false);
	void restartLevel();
	void onPlayerDied();
	void saveData();
	void loadSaveFile();
	bool m_loadingFile = false;

	static void delptr();
};