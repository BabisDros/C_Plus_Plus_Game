#include "UIManager.h"
#include "util.h"
#include "Player.h"
#include "GameEvents.h"
#include "MusicManager.h"

void UIManager::init()
{
	m_state = GameState::getInstance();
	setCustomBrushProperties(&m_menu, 1, 0, m_state->getFullAssetPath("UI\\main_menu_alt.png"));

	GameEvents::getInstance()->m_playerHealthChanged.addArgActionCallback(std::bind(&UIManager::onPlayerHealthChanged, this, std::placeholders::_1, std::placeholders::_2));
	GameEvents::getInstance()->m_pointsChanged.addArgActionCallback(std::bind(&UIManager::onPointsChanged, this));
	GameEvents::getInstance()->m_playerLivesChanged.addArgActionCallback(std::bind(&UIManager::onPlayerLivesChanged, this));
	m_playerHealthUI = HealthUIFixed(0, 0, 5, 1);
	 
	m_lostEffect = new ParticleSystem(10, 1000, m_state->getCanvasWidth() / 2, 0, m_state->getCanvasWidth(), 0.3f, 50.f,
		m_state->getFullAssetPath("Effects\\blood.png"), 10.f, 5.f, 5.f, 0.f, 0.0f);
	m_winEffect = new ParticleSystem(20, 1000, m_state->getCanvasWidth() / 2, m_state->getCanvasHeight()/4, 2.3f, 0.3f, 50.f,
		m_state->getFullAssetPath("Effects\\smoke3.png"), 10.f, 5.f, 5.f, 0.f, 0.0f, 1.f, 1.f, 0);

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
	setCustomBrushProperties(&arrowBrush, 1.f, 0, m_state->getFullAssetPath("UI\\arrow-right.png"));
	setCustomBrushProperties(&helpBrush, 1.f, 0);
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

void UIManager::update(const float& dt)
{
	if (m_state->getCurrentState() == Win)
	{
		m_winEffect->init();
		m_winEffect->update(dt);
		m_star.update(dt);		
	}
	else if (m_state->getCurrentState() == Lose)
	{
		m_lostEffect->init();
		m_lostEffect->update(dt);
	}
	else m_star.init();
}

UIManager::~UIManager()
{
	//initialized with "new" in init
	delete m_lostEffect;
	delete m_winEffect;
}

void UIManager::drawPlayerHealth()
{	
	m_playerHealthUI.draw();	
}

void UIManager::drawScore()
{
	//although score runs in update before drawPause, it continues to draw above it
	m_scoreTxt = "Score: " + m_pointsTxt;
	graphics::drawRect(m_state->getCanvasWidth() - 1.8f, 0.75f, 3.4f, 1, backPLate);
	graphics::drawText(m_state->getCanvasWidth() - 3.2f, 1.f, .6f, m_scoreTxt, textBrush);	
}

void UIManager::drawPause()
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
	graphics::drawText(0.125f, m_playerHealthUI.getHeight() + backgroungUiHeight / 2+ calcCenteringYForTextSize(m_livesFontSize), m_livesFontSize, m_livesDisplayTxt, textBrush);
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
		graphics::setScale(-1.f, 1.f);
		graphics::drawRect(m_state->getCanvasWidth() / 8, m_state->getCanvasHeight() - 2.f, 2.f, 1.f, arrowBrush);
		graphics::resetPose();
	}

	graphics::drawRect(m_state->getCanvasWidth() * 7 / 8, m_state->getCanvasHeight() - 2.f, 2.f, 1.f, arrowBrush);

	m_state->enable(m_pressed_left, m_left_held, graphics::getKeyState(graphics::SCANCODE_LEFT));
	m_state->enable(m_pressed_right, m_right_held, graphics::getKeyState(graphics::SCANCODE_RIGHT));

	if (m_curr_left != m_pressed_left && m_help_page > 1)
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
		drawHelpExtra();
	}
	else
	{
		m_help_page = 1; // reset page count
		m_state->setState(Menu);
	}

	
	std::string m_pagetxt = std::to_string(m_help_page) + "/2";
	float centeringValueX = calcCenteringXForTextSize(m_pagetxt, 1.f);

	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() - .5f, 1.f, m_pagetxt, textBrush);
}

void UIManager::drawHelpBasicMovement()
{
	float centeringValueX = calcCenteringXForTextSize(m_helpMvTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3, m_helpTxtFontSize, m_helpMvTxt, textBrush);
	helpBrush.texture = m_state->getFullAssetPath("UI\\arrow-keys.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 3 , 4, 2, helpBrush);

	centeringValueX = calcCenteringXForTextSize(m_helpDashTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3 + 3 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpDashTxt, textBrush);
	helpBrush.texture = m_state->getFullAssetPath("UI\\key-z.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 3 + 2.7f * m_helpTxtFontSize, 1, 1, helpBrush);

	centeringValueX = calcCenteringXForTextSize(m_helpAttackTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3 + 6 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpAttackTxt, textBrush);
	helpBrush.texture = m_state->getFullAssetPath("UI\\key-x.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 3 + 5.7f * m_helpTxtFontSize, 1, 1, helpBrush);

}

void UIManager::drawHelpExtra()
{
	float centeringValueX = calcCenteringXForTextSize(m_helpPauseTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3, m_helpTxtFontSize, m_helpPauseTxt, textBrush);
	helpBrush.texture = m_state->getFullAssetPath("UI\\key-p.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 3 - 0.2f, 1, 1, helpBrush);

	centeringValueX = calcCenteringXForTextSize(m_helpDevFlyTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3 + 3 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpDevFlyTxt, textBrush);
	helpBrush.texture = m_state->getFullAssetPath("UI\\key-9.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 3 + 2.7f * m_helpTxtFontSize, 1, 1, helpBrush);

	centeringValueX = calcCenteringXForTextSize(m_helpDevViewTxt, m_helpTxtFontSize);
	graphics::drawText(m_state->getCanvasWidth() / 2 - centeringValueX, m_state->getCanvasHeight() / 3 + 6 * m_helpTxtFontSize, m_helpTxtFontSize, m_helpDevViewTxt, textBrush);
	helpBrush.texture = m_state->getFullAssetPath("UI\\key-8.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 3 + 5.7f * m_helpTxtFontSize, 1, 1, helpBrush);
}