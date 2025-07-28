#pragma once
#include "Singleton.h"
#include <vector>
#include <string>

class LevelManager: public Singleton<LevelManager>
{
	class GameState* m_state;
	float m_animation_timer = 0;
	bool m_restartAfterDeath = false;
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
	void loadSavedFile();
	void levelEndCutscene();
	void levelStartCutscene();
	bool m_loadingFile = false;
	bool m_cutscene_move_player = false;
	bool m_cutscene_ended = true;
	void setRestartAfterDeath(bool value);
};