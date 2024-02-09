#pragma once
#include "Manager.h"
#include <vector>
#include <string>

class LevelManager: public Manager<LevelManager>
{
	class GameState* m_state;
	float m_animation_timer = 0;
public:
	int m_level_counter = -1; // starts counting from 0
	std::vector<std::string> levels_list;
	bool m_restart = false;
	void init();
	void update(const float& dt);
	void nextLevel(bool restartLevel = false);
	void restartLevel();
	void onPlayerDied();
	void saveData();
	void loadSaveFile();
	void levelEndCutscene();
	void levelStartCutscene();
	bool m_loadingFile = false;
	bool m_cutscene_move_player = false;
	bool m_cutscene_ended = true;
};