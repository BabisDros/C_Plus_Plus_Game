#include "UIManager.h"
#include "Util.h"
#include "Gamestate.h"
//#include "Player.h"
#include <iostream>
#include "CallbackManager.h"
UIManager* UIManager::s_unique_instance = nullptr;

void UIManager::init()
{
	m_state = GameState::getInstance();
	setCustomBrushProperties(&m_menu, 1, 0, m_state->getFullAssetPath("UI\\main_menu_alt.png"));

	CallbackManager::getInstance()->m_playerIsDamaged.addActionCallbackTwoInt(std::bind(&UIManager::onPlayerHealthChanged, this, std::placeholders::_1, std::placeholders::_2));
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


void UIManager::drawPause()	//! make it better than a greyed out screen
{
	graphics::Brush paused_brush;
	paused_brush.fill_opacity = 0.75f;
	paused_brush.outline_opacity = 0.f;
	SETCOLOR(paused_brush.fill_color, 0.25f, 0.25f, 0.25f);

	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), paused_brush);

	std::string str = "Paused";
	graphics::Brush textBrush;
	SETCOLOR(textBrush.outline_color, 1, 0.1f, 0);
	textBrush.fill_opacity = 1.f;
	textBrush.outline_opacity = 1.0f;
	float centeringValue = str.size() / 4.f;//centering offset value for 1 size font, each letter is half a unit
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValue, m_state->getCanvasHeight() / 2, 1.f, str, textBrush);
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

void UIManager::onPlayerHealthChanged(const int& initialHealth, const int& currentHealth)
{
	m_playerHealthUI->updateUIOnDamage(initialHealth, currentHealth);
}

void UIManager::drawMenu()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), m_menu);
}



UIManager* UIManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new UIManager();
	}
	return s_unique_instance;
}
