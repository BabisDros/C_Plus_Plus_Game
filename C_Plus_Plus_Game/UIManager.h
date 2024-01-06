#pragma once
#include <string>
#include <sgg/graphics.h>
#include "HealthUIFixed.h"
class UIManager
{
	class GameState* m_state;
	static UIManager* s_unique_instance;
	HealthUIFixed* m_playerHealthUI = new HealthUIFixed(0,0,5,1);
	graphics::Brush m_menu, m_healthTxt;
	std::string m_playerHealthStr;

public:
	void init();
	void draw();
	void drawPlayerHealth();
	void drawMenu();
	void drawText();
	void drawPause();
	void onPlayerHealthChanged(const int& initialHealth, const int& currentHealth);
	static UIManager* getInstance();
};
