#pragma once
#include <string>
#include "HealthUIFixed.h"
#include "Star.h"

class UIManager
{
	class GameState* m_state;
	static UIManager* s_unique_instance;
	HealthUIFixed* m_playerHealthUI = new HealthUIFixed(0,0,5,1);
	std::string m_points;
	graphics::Brush m_menu;
	Star star;
public:
	int m_fps=0;
	void init();
	void draw();
	void update(float dt);
	void drawPlayerHealth();
	void drawScore();
	void drawMenu();
	void drawPause();
	void drawWinScreen();
	void drawLoseScreen();
	void drawDashCooldown();
	void drawFps();
	void onPlayerHealthChanged(const int& initialHealth, const int& currentHealth);
	void onPointsChanged(const int& points);
	static UIManager* getInstance();
};
