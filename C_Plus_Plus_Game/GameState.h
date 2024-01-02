#pragma once
#include "sgg/graphics.h"
#include <string>

class GameState
{
	std::string m_asset_path = "assets\\";
	std::string m_data_path = "data\\";
	float m_canvas_width = 24.0f;
	float m_canvas_height = m_canvas_width / 2;

	static GameState* s_unique_instance;
	
	GameState();
	float m_pausableClock = 0.0f;
	class Player* m_player = 0;
	class Level* m_current_level = 0;

public:	
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	bool m_debugging = false;
	bool m_debugging_held = false;

	bool m_paused = false;
	bool m_paused_held = false;

	int m_fps = 0;
	int m_time = 0; // used for fps counting purposes

	void init();
	void draw();
	void update(float dt);
	float* getPausableClock() { return &m_pausableClock; };
	static GameState* getInstance();
	~GameState();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	void showFPS();

	void enable(bool& m_option, bool& m_option_held, bool m_button);

	std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	std::string getFullDataPath(const std::string& data);

	Player* getPlayer() { return m_player; }
	Level* getLevel() { return m_current_level; }
	
};