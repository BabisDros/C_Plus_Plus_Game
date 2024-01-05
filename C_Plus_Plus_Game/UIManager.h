#pragma once
#include <string>
#include <sgg/graphics.h>

class UIManager
{
	class GameState* m_state;
	static UIManager* s_unique_instance;
	graphics::Brush menu;
	graphics::Brush playerHealth;
public:
	void init();
	void draw();
	static UIManager* getInstance();
};
