#pragma once
#include <string>
#include <sgg/graphics.h>
#include "HealthUIFixed.h"
class UIManager
{
	class GameState* m_state;
	static UIManager* s_unique_instance;
	HealthUIFixed* playerHealthUI=new HealthUIFixed(0,0,5,1);
	graphics::Brush menu,healthTxt;
	std::string playerHealthStr;

public:
	void init();
	void draw();
	void drawPlayerHealth();
	void drawMenu();
	void drawText();
	void drawPause();
	void onPlayerHealthChanged();
	static UIManager* getInstance();
};
