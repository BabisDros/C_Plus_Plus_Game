#include "GameState.h"
#include "Level.h"
#include "Player.h"
#include <thread>
#include <chrono>
#include <iostream>

GameState::GameState()
{
}

void GameState::init()
{
	m_current_level = new Level();
	m_current_level->init();

	m_player = new Player("Player");
	m_player->init();

	graphics::preloadBitmaps(getAssetDir()); //? preload assets
	//graphics::setFont(m_asset_path + "path");		//?	adds font
}

void GameState::draw()
{
	if (!m_current_level) return;

	m_current_level->draw();
}

void GameState::update(float dt)
{
	if (dt > 200) return;	//? it been too long since last frame

	/* fixes screenshaking, basically reducing frames, no issues currently, no need to enable it
	float sleep_time = std::max(0.0f, 17.0f - dt);
	std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));*/ 

	if (!m_current_level) return;
	
	if (!m_paused) m_current_level->update(dt);
	else m_current_level->pausedDraw();	//? Make paused window

	enable(m_debugging, m_debugging_held, graphics::getKeyState(graphics::SCANCODE_0));
	enable(m_paused, m_paused_held, graphics::getKeyState(graphics::SCANCODE_P));
	showFPS();
}

GameState* GameState::getInstance()
{
	if (m_unique_instance == nullptr)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

GameState::~GameState()
{
	if (m_player)
	{
		delete m_player;
	}
	if (m_current_level)
	{
		delete m_current_level;
	}
}

void GameState::showFPS()
{
	int currentTime = graphics::getGlobalTime() / 1000;
	if (currentTime > m_time)
	{
		std::cout << m_fps << "\n";
		m_fps = 0;
		while (currentTime > m_time)
		{
			m_time++;
		}
	}
	else
	{
		m_fps++;
	}
}


/** Generic method to switch between modes on every button press
	\param m_option = var that enables/disables mode
	\param m_option_held = var to check if it is held
	\param m_button = binded key press {graphics::getKeyState(graphics:: }
*/
void GameState::enable(bool& m_option, bool& m_option_held, bool m_button)
{
	if (m_button)	// changes only on next key press
	{
		if (!m_option_held)
		{
			m_option = !m_option;
			m_option_held = true;
		}
	}
	else
	{
		m_option_held = false;
	}
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

/** Used for Level data	
*/
std::string GameState::getFullDataPath(const std::string& data)
{
	return m_data_path + data;
}



GameState* GameState::m_unique_instance = nullptr;