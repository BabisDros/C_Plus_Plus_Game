#pragma once
#include <vector>
#include <string>
class LevelManager
{
	class GameState* m_state;
	static LevelManager* s_unique_instance;
	int m_level_counter = -1; // starts counting from 0
	bool m_win = false;
public:
	std::vector<std::string> levels_list;
	void init();
	static LevelManager* getInstance();
	void nextLevel();
	void saveData();
	void loadSaveFile();
	bool m_loadingFile = false;
};