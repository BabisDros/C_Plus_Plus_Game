#include "UIManager.h"
#include "Util.h"
#include "Gamestate.h"
//#include "Player.h"
#include <iostream>

UIManager* UIManager::s_unique_instance = nullptr;

void UIManager::init()
{
	m_state = GameState::getInstance();
	//std::cout << "calllll"<< m_state->getFullAssetPath("UI\\main_menu_alt.png");
	setCustomBrushProperties(&menu, 1, 0, m_state->getFullAssetPath("UI\\main_menu_alt.png"));	
	/*playerHealth->setSize(5.f, 1.f);
	playerHealth->setPosition(0.f, 0.f);*/
}


void UIManager::draw()
{
	if (m_state->getCurrentState() == States::Menu)
	{
		drawMenu();
	}
	else
	{
		drawPlayerHealth();
	}
	//std::cout << m_state->getCurrentState();
}

void UIManager::drawPlayerHealth()
{
	playerHealthUI->draw();
}

void UIManager::drawText()
{
	//SETCOLOR(healthTxt.outline_color, 1, 0.1f, 0);
	//healthTxt.fill_opacity = 1.f;
	//healthTxt.outline_opacity = 1.0f;
	//playerHealthStr = std::to_string(m_state->getPlayer()->getHealth());
	//float centeringValue = playerHealthStr.size() / 25.f;//centering offset value for 0.2 size font
	//graphics::drawText(0 - centeringValue, 0, 0.2f, playerHealthStr, healthTxt);
}

void UIManager::onPlayerHealthChanged()
{
}

void UIManager::drawMenu()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), menu);
}



UIManager* UIManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new UIManager();
	}
	return s_unique_instance;
}
