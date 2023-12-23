#pragma once
#include "sgg/graphics.h"
#include <string>

class GameState
{
	std::string m_asset_path = "assets\\";
	std::string m_data_path = "data\\";
	float m_canvas_width = 16.0f;
	float m_canvas_height = 8.0f;

	static GameState* m_unique_instance;

	GameState();

	class Player* m_player = 0;
	class Level* m_current_level = 0;

public:
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	bool m_debugging = false;
	bool m_debugging_held = false;
	int fps = 0;
	int time = 0; // used for fps counting purposes

	void init();
	void draw();
	void update(float dt);

	static GameState* getInstance();
	~GameState();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	void showFPS();

	std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	std::string getFullDataPath(const std::string& data);

	class Player* getPlayer() { return m_player; }
};