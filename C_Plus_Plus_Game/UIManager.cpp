#include "UIManager.h"
#include "Util.h"
#include "Gamestate.h"
//#include "Player.h"
#include <iostream>
#include "CallbackManager.h"
#include "player.h"
#include "MusicManager.h"

UIManager* UIManager::s_unique_instance = nullptr;

void UIManager::init()
{
	m_state = GameState::getInstance();
	setCustomBrushProperties(&m_menu, 1, 0, m_state->getFullAssetPath("UI\\main_menu_alt.png"));

	CallbackManager::getInstance()->m_playerHealthChanged.addArgActionCallback(std::bind(&UIManager::onPlayerHealthChanged, this, std::placeholders::_1, std::placeholders::_2));
	CallbackManager::getInstance()->m_pointsChanged.addArgActionCallback(std::bind(&UIManager::onPointsChanged, this));
	CallbackManager::getInstance()->m_playerLivesChanged.addArgActionCallback(std::bind(&UIManager::onPlayerLivesChanged, this));
	m_playerHealthUI = HealthUIFixed(0, 0, 5, 1);
	 
	m_lostEffect = new ParticleSystem(10, 1000, m_state->getCanvasWidth() / 2, 0, m_state->getCanvasWidth(), 0.3f, 50.f,
		m_state->getFullAssetPath("blood.png"), 10.f, 5.f, 5.f, 0.f, 0.0f);
	m_winEffect = new ParticleSystem(20, 1000, m_state->getCanvasWidth() / 2, m_state->getCanvasHeight()/4, 2.3f, 0.3f, 50.f,
		m_state->getFullAssetPath("smoke3.png"), 10.f, 5.f, 5.f, 0.f, 0.0f, 1.f, 1.f, 0);

	SETCOLOR(backPLate.fill_color, 0, 0, 0);
	backPLate.fill_opacity = 0.5f;
	SETCOLOR(backPLate.outline_color, 0, 0, 0)
		
	setCustomBrushProperties(&paused_brush, 0.8f, 0, m_state->getFullAssetPath("UI\\pause.png"));

	SETCOLOR(textBrush.outline_color, 1, 0.1f, 0);
	textBrush.fill_opacity = 1.f;
	textBrush.outline_opacity = 1.0f;

	setCustomBrushProperties(&winBrush, 0.8f, 0, m_state->getFullAssetPath("UI\\pause.png"));

	SETCOLOR(diedBrush.fill_color, 0.3f, 0.3f, 0.3f);
	setCustomBrushProperties(&diedBrush, 0.9f, 0, m_state->getFullAssetPath("UI\\pause.png"));
	setCustomBrushProperties(&skullBrush, 0.9f, 0, m_state->getFullAssetPath("UI\\skull.png"));
	setCustomBrushProperties(&arrowBrush, 1.f, 0, m_state->getFullAssetPath("UI\\left arrow.png"));
}

//text always run above drawrect
void UIManager::draw()
{
	if (m_state->getCurrentState() == States::Menu)
	{
		drawMenu();
	}
	else if (m_state->getCurrentState() == States::Help)
	{
		drawHelp();
	}
	else
	{
		if (m_state->getCurrentState() == States::InGame)
		{
			drawPlayerHealth();
			drawScore();
			drawDashCooldown();
			drawFps();
			drawLives();
		}
		
		if (m_state->getCurrentState() == States::Paused)
		{			
			drawPause();		
		}

		else if(m_state->getCurrentState() == States::Lose)
		{
			drawLoseScreen();
		}

		else if (m_state->getCurrentState() == States::Win)
		{
			drawWinScreen();
		}
	}
}

void UIManager::update(float dt)
{
	if (m_state->getCurrentState() == Win)
	{
		m_winEffect->init();
		m_winEffect->update(dt, true);
		m_star.update(dt);		
	}
	else if (m_state->getCurrentState() == Lose)
	{
		m_lostEffect->init();
		m_lostEffect->update(dt, true);
	}
	else m_star.init();
}

void UIManager::drawPlayerHealth()
{	
	m_playerHealthUI.draw();	
}

void UIManager::drawScore()
{
	//although score runs in update before drawPause, it continues to draw above it
	m_scoreTxt = "Score: " + m_pointsTxt;
	graphics::drawRect(m_state->getCanvasWidth() - 2, 0.75f, 3, 1, backPLate);
	graphics::drawText(m_state->getCanvasWidth() - 3.2f, 1.f, .6f, m_scoreTxt, textBrush);	
}

void UIManager::drawPause()	//! make it better than a greyed out screen
{	
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), paused_brush);
	
	float centeringValueX = calcCenteringXForTextSize(m_pausedTxt, 1.f);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 2, m_pausedTxtFontSize, m_pausedTxt, textBrush);

	float centeringValueX2 = calcCenteringXForTextSize(m_escText, m_escTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX2, m_state->getCanvasHeight() / 2 + m_pausedTxtFontSize, m_escTxtFontSize, m_escText, textBrush);

	//draw text Paused
	float centeringValueX3 = calcCenteringXForTextSize(m_restartLevelTxt, 0.5f);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX3, m_state->getCanvasHeight() / 2 +m_pausedTxtFontSize+ m_escTxtFontSize,
		m_escTxtFontSize, m_restartLevelTxt, textBrush);
}
void UIManager::drawLives()
{
	m_livesDisplayTxt = "Lives: " + m_livesTxt;
	
	float backgroungUiHeight=1;
	graphics::drawRect(0, m_playerHealthUI.getHeight()+ backgroungUiHeight / 2, m_livesDisplayTxt.size()/1.6f, backgroungUiHeight, backPLate);
	graphics::drawText(0, m_playerHealthUI.getHeight() + backgroungUiHeight / 2+ calcCenteringYForTextSize(m_livesFontSize), m_livesFontSize, m_livesDisplayTxt, textBrush);
}
void UIManager::drawWinScreen()
{			
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), m_state->getCanvasHeight(), winBrush);		
	m_star.draw();
	if (!m_star.hasGrown()) return;

	MusicManager::getInstance()->playWinSound();

	
	m_winEffect->draw(false);
	float centeringValueX = calcCenteringXForTextSize(m_winTxt, 1.f);//centering offset value for 1 size font, each letter is half a unit
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 2, m_winTxtFontSize, m_winTxt, textBrush);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 2+ m_winTxtFontSize, m_winTxtFontSize, m_scoreTxt, textBrush);
}

void UIManager::drawLoseScreen()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), m_state->getCanvasHeight(), diedBrush);
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 4, 4, 4, skullBrush);
	m_lostEffect->draw(false);

	float centeringValueX = calcCenteringXForTextSize(m_loseTxt, 1.f);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 2, m_loseTxtFontSize, m_loseTxt, textBrush);

	float centeringValueX2 = calcCenteringXForTextSize(m_escText, m_escTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX2, m_state->getCanvasHeight() / 2 + m_pausedTxtFontSize, m_escTxtFontSize, m_escText, textBrush);

	//draw text Paused
	float centeringValueX3 = calcCenteringXForTextSize(m_restartGameTxt, 0.5f);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX3, m_state->getCanvasHeight() / 2 + m_pausedTxtFontSize + m_escTxtFontSize,
		m_escTxtFontSize, m_restartGameTxt, textBrush);
}
void UIManager::drawDashCooldown()
{
	graphics::Brush cooldown, on_cooldown;
	setCustomBrushProperties(&cooldown, 1, 1, m_state->getFullAssetPath("UI\\dash_cd.png"));
	bool onCD = false;
	float height;
	if (m_state->getPlayer()->getDashAbility()->getStartTime() != 0)
	{
		//setCustomBrushProperties(&cooldown, 1, 1, m_state->getFullAssetPath("Character Sprites V2\\Run\\Run3.png"));
		SETCOLOR(cooldown.fill_color, 0.7, 0.7, 0.7);
		cooldown.outline_opacity = 1;
		SETCOLOR(on_cooldown.fill_color, 0.6, 0.6, 0.6);
		on_cooldown.fill_opacity = 0.5f;
		height = 1-(m_state->getPlayer()->getDashAbility()->getElapsedTime()/m_state->getPlayer()->getDashAbility()->getCooldown());
		onCD = true;
	}
	graphics::drawRect(m_state->getCanvasWidth() - 1, 3, 1,	1, cooldown);
	if (onCD)graphics::drawRect(m_state->getCanvasWidth() - 1, 3 - height / 2 + 0.5, 1, height, on_cooldown);	// y = location - go top to bottom + image height /2
}

void UIManager::drawFps()
{
	m_fpsTxt = std::to_string(m_fps);
	SETCOLOR(textBrush.outline_color, 1, 1, 1);
	graphics::drawText(0.2f, m_state->getCanvasHeight() - .5f, .4f, m_fpsTxt, textBrush);
}

void UIManager::drawMenu()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), m_menu);
}

void UIManager::onPlayerHealthChanged(const int& initialHealth, const int& currentHealth)
{	
	m_playerHealthUI.updateUIOnDamage(initialHealth, currentHealth);		
}
void UIManager::onPlayerLivesChanged()
{
	m_livesTxt = std::to_string(m_state->m_lives);
}

void UIManager::onPointsChanged()
{
	m_pointsTxt = std::to_string(m_state->m_points);
}

void UIManager::drawHelp()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(),
		m_state->getCanvasHeight(), paused_brush);
	bool m_curr_left = m_pressed_left, m_curr_right = m_pressed_right;

	if (m_help_page > 1)
	{
		graphics::drawRect(m_state->getCanvasWidth() / 8, m_state->getCanvasHeight() - 2.f, 2.f, 2.f, arrowBrush);
	}

	graphics::setScale(-1.f, 1.f);
	graphics::drawRect(m_state->getCanvasWidth() * 7 / 8, m_state->getCanvasHeight() - 2.f, 2.f, 2.f, arrowBrush);
	graphics::resetPose();

	m_state->enable(m_pressed_left, m_left_held, graphics::getKeyState(graphics::SCANCODE_LEFT));
	m_state->enable(m_pressed_right, m_right_held, graphics::getKeyState(graphics::SCANCODE_RIGHT));

	if (m_curr_left != m_pressed_left && m_help_page > 0)
	{
		--m_help_page;
		m_curr_left = m_pressed_left;
	}

	if (m_curr_right != m_pressed_right)
	{
		++m_help_page;
		m_curr_right = m_pressed_right;
	}

	if (m_help_page == 1)
	{
		drawHelpBasicMovement();
	}
	else if (m_help_page == 2)
	{
		drawHelpAbilities();
	}
	else if (m_help_page == 3)
	{
		drawHelpExtra();
	}
	else
	{
		m_help_page = 1; // reset page count
		m_state->setState(Menu);
	}

	
	std::string m_pagetxt = std::to_string(m_help_page) + "/3";
	float centeringValueX = calcCenteringXForTextSize(m_pagetxt, 1.f);

	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() - .5f, 1.f, m_pagetxt, textBrush);
}

void UIManager::drawHelpBasicMovement()
{
	float centeringValueX = calcCenteringXForTextSize(m_helpLeftTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3, m_helpTxtFontSize, m_helpLeftTxt, textBrush);

	float centeringValueX2 = calcCenteringXForTextSize(m_helpRightTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX2, m_state->getCanvasHeight() / 3 + 2 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpRightTxt, textBrush);

	float centeringValueX3 = calcCenteringXForTextSize(m_helpJumpTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX3, m_state->getCanvasHeight() / 3 + 4 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpJumpTxt, textBrush);
	
	// draw animations

	//m_brush.texture = (*m_sprites_ptr).at((int)(8 * dif) % (*m_sprites_ptr).size());
}

void UIManager::drawHelpAbilities()
{
	float centeringValueX = calcCenteringXForTextSize(m_helpDashTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3, m_helpTxtFontSize, m_helpDashTxt, textBrush);

	float centeringValueX2 = calcCenteringXForTextSize(m_helpAttackTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX2, m_state->getCanvasHeight() / 3 + 2 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpAttackTxt, textBrush);
}

void UIManager::drawHelpExtra()
{
	float centeringValueX = calcCenteringXForTextSize(m_helpPauseTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3, m_helpTxtFontSize, m_helpPauseTxt, textBrush);

	float centeringValueX2 = calcCenteringXForTextSize(m_helpDevFlyTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX2, m_state->getCanvasHeight() / 3 + 2 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpDevFlyTxt, textBrush);

	float centeringValueX3 = calcCenteringXForTextSize(m_helpDevViewTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX3, m_state->getCanvasHeight() / 3 + 4 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpDevViewTxt, textBrush);
}

UIManager* UIManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new UIManager();
	}
	return s_unique_instance;
}
