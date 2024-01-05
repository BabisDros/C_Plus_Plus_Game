#include "UIManager.h"
#include "Util.h"
#include "Gamestate.h"
#include <iostream>
UIManager* UIManager::s_unique_instance = nullptr;

void UIManager::init()
{
	m_state = GameState::getInstance();
	//std::cout << "calllll"<< m_state->getFullAssetPath("UI\\main_menu_alt.png");
	setCustomBrushProperties(&menu, 1, 0, m_state->getFullAssetPath("UI\\main_menu_alt.png"));	
}

void UIManager::draw()
{
	if (m_state->getCurrentState() == States::Menu)
	{
		drawMenu();
	}
}

void UIManager::drawPlayerHealth()
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
