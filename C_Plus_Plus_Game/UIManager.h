#pragma once
#include <string>
#include "HealthUIFixed.h"
#include "Star.h"
#include "ParticleSystem.h"

class UIManager
{
	static UIManager* s_unique_instance;
	class GameState* m_state=nullptr;
	ParticleSystem* m_lostEffect = nullptr;
	ParticleSystem* m_winEffect = nullptr;
	HealthUIFixed m_playerHealthUI;
	Star m_star;
	std::string m_pointsTxt="";
	std::string m_scoreTxt = "";
	std::string m_livesTxt = "";
	std::string m_livesDisplayTxt = "";
	std::string m_pausedTxt = "Paused";
	std::string m_escText = "Press  \"Esc\"  to exit";
	std::string m_restartTxt = "Press  \"R\"  to  Restart";
	std::string m_winTxt = "You  won!";
	std::string m_loseTxt = "You  lose!";
	std::string m_fpsTxt = "";

	graphics::Brush m_menu;
	graphics::Brush textBrush;
	graphics::Brush backPLate;
	graphics::Brush paused_brush;
	graphics::Brush winBrush;
	graphics::Brush diedBrush;
	graphics::Brush skullBrush;
	float m_pausedTxtFontSize = 1.0f;
	float m_escTxtFontSize = 0.5f;
	float m_winTxtFontSize = 1.0f;
	float m_loseTxtFontSize = 1.0f;
	float m_livesFontSize = 0.6f;

	
public:
	int m_fps=0;
	void init();
	void draw();
	void update(float dt);
	void drawPlayerHealth();
	void drawScore();
	void drawMenu();
	void drawPause();
	void drawLives();
	void drawWinScreen();
	void drawLoseScreen();
	void drawDashCooldown();
	void drawFps();
	void onPlayerHealthChanged(const int& initialHealth, const int& currentHealth);
	void onPlayerLivesChanged();
	void onPointsChanged();
	static UIManager* getInstance();
};
