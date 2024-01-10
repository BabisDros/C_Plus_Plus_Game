#include "UIManager.h"
#include "Util.h"
#include "Gamestate.h"
//#include "Player.h"
#include <iostream>
#include "CallbackManager.h"
#include "player.h"
UIManager* UIManager::s_unique_instance = nullptr;

void UIManager::init()
{
	m_state = GameState::getInstance();
	setCustomBrushProperties(&m_menu, 1, 0, m_state->getFullAssetPath("UI\\main_menu_alt.png"));

	CallbackManager::getInstance()->m_playerIsDamaged.addTwoArgActionCallback(std::bind(&UIManager::onPlayerHealthChanged, this, std::placeholders::_1, std::placeholders::_2));
	CallbackManager::getInstance()->m_pointsChanged.addArgActionCallback(std::bind(&UIManager::onPointsChanged, this, std::placeholders::_1));
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
		drawScore();
		if (m_state->getCurrentState() == States::Paused)
		{
			drawPause();
		}
	}
}

void UIManager::drawPlayerHealth()
{
	m_playerHealthUI->draw();
}

void UIManager::drawScore()
{
	std::string str = "Score: " + m_points;
	graphics::Brush textBrush;
	SETCOLOR(textBrush.outline_color, 1, 0.1f, 0);
	textBrush.fill_opacity = 1.f;
	textBrush.outline_opacity = 1.0f;
	float centeringValue = str.size() / 3.f;//centering offset value for 1 size font, each letter is half a unit
	graphics::drawText(m_state->getCanvasWidth() - centeringValue, 1.f, .6f, str, textBrush);
}


void UIManager::drawPause()	//! make it better than a greyed out screen
{
	//draw canvas
	graphics::Brush paused_brush;
	paused_brush.fill_opacity = 0.75f;
	paused_brush.outline_opacity = 0.f;
	SETCOLOR(paused_brush.fill_color, 0.25f, 0.25f, 0.25f);

	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), paused_brush);
	
	//draw text
	std::string str = "Paused";
	graphics::Brush textBrush;
	SETCOLOR(textBrush.outline_color, 1, 0.1f, 0);
	textBrush.fill_opacity = 1.f;
	textBrush.outline_opacity = 1.0f;
	float centeringValue = str.size() / 4.f;//centering offset value for 1 size font, each letter is half a unit
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValue, m_state->getCanvasHeight() / 2, 1.f, str, textBrush);
}

void UIManager::drawMenu()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), m_menu);
}

void UIManager::onPlayerHealthChanged(const int& initialHealth, const int& currentHealth)
{
	m_playerHealthUI->updateUIOnDamage(initialHealth, currentHealth);
}

void UIManager::onPointsChanged(const int& points)
{
	m_points = std::to_string((int)points);
}



UIManager* UIManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new UIManager();
	}
	return s_unique_instance;
}
