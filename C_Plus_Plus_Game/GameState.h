#pragma once
#include "sgg/graphics.h"
#include <string>
#include <list>
#include "timer.h"
#include "Ability.h"

enum States
{
	Menu,
	InGame,
	Paused,
	Lose,
	Win,
	Help
};

class GameState
{
	std::string m_asset_path = "assets\\";
	std::string m_data_path = "data\\";
	float m_canvas_width = 24.0f;
	float m_canvas_height = m_canvas_width / 2;
	int m_initialLives = 2;
	int m_initialHealth = 100;
	static GameState* s_unique_instance;
	static States m_currentState;
	GameState();
	float m_pausableClock = 0.0f;
	
	Ability timer = Ability(1.0f, 0.25f, 0.0f);
public:	
	class Player* m_player = 0;
	class Level* m_current_level = 0;

	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	bool m_debugging = false;
	bool m_debugging_held = false;

	/*paused is used to pause inGame update loops.
	All other updates continue to run*/
	bool m_suspendExecution = false;
	bool m_paused_held = false;
	bool m_pauseButtonPressed = false;
	bool m_goNextLevel = false;

	int m_fps = 0;
	int m_time = 0; // used for fps counting purposes
	
	int m_lives = m_initialLives;
	int m_points = 0 ;

	void init();
	void draw();
	void update(float dt);
	float* getPausableClock() { return &m_pausableClock; };
	static GameState* getInstance();
	~GameState();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	void handleStates();
	void setState(States state);
	void deletePlayer() const;
	void showFPS();
	void onPointsCollected(int points);
	void onPlayerLivesChanged(int life);
	States& getCurrentState();
	void enable(bool& m_option, bool& m_option_held, bool m_button);

	std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	std::string getFullDataPath(const std::string& data);

	Player* getPlayer() { return m_player; }
	class Level* getLevel() { return m_current_level; }
	int getInitialLives() const;
	int getInitialHealth() const;
};
