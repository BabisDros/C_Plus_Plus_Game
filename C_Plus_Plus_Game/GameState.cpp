#pragma once
#include "UIManager.h"
#include "GameState.h"
#include "Level.h"
#include "Player.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <filesystem>


GameState::GameState()
{
}

void GameState::init()
{	
	std::string path = getFullDataPath("");
	for (const auto& entry : std::filesystem::directory_iterator(path))	// list of level names on data folder
		levels.push_back(entry.path().u8string().erase(entry.path().u8string().find(".txt"), 4).	// remove .txt extention
		erase(0, getFullDataPath("").size()));	// remove parent directory

	
	UIManager::getInstance()->init();
	
	graphics::preloadBitmaps(getAssetDir()); //? preload assets
	//graphics::setFont(m_asset_path + "path");		//?	adds font
}

void GameState::draw()
{
	
	if (m_currentState == Menu)
	{
		UIManager::getInstance()->draw();
	}

	if (!m_current_level) return;

	m_current_level->draw();
	
}



void GameState::update(float dt)
{
	if (dt > 200) return;	//? it been too long since last frame

	/* fixes screenshaking, basically reducing frames, no issues currently, no need to enable it
	float sleep_time = std::max(0.0f, 17.0f - dt);
	std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));*/ 
	handleStates();
	if (!m_current_level) return;
	
	if (!m_paused)
	{
		m_current_level->update(dt);
		m_pausableClock += graphics::getDeltaTime()/1000;
	}
	else
	{
		m_current_level->pausedDraw();	//? Make paused window
	}
	
	enable(m_debugging, m_debugging_held, graphics::getKeyState(graphics::SCANCODE_0));
	enable(m_paused, m_paused_held, graphics::getKeyState(graphics::SCANCODE_P));
	showFPS();
	if (goNextLevel) nextLevel();
}

GameState* GameState::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new GameState();
	}
	return s_unique_instance;
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

void GameState::handleStates()
{
	if (m_currentState == Menu && graphics::getKeyState(graphics::SCANCODE_N))
	{
		if (graphics::getKeyState(graphics::SCANCODE_N))
		{
			m_current_level = new Level(levels.front());
			levels.pop_front();
			m_current_level->init();
			m_player = new Player("Player", 100);
			m_player->init();

			m_currentState = States::InGame;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_L))
		{

		}
	}
}

void GameState::showFPS()
{
	float currentTime = graphics::getGlobalTime() / 1000;
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

void GameState::nextLevel()
{
	m_current_level->~Level();
	m_current_level = new Level(levels.front());
	m_current_level->init();
	if (levels.size()>1) levels.pop_front();	//? If the isn't at least 1 more level, next tp instance will result in crash
	m_player->init();
	graphics::preloadBitmaps(getAssetDir()); //! preload assets, not needed if all assets in 1 folder
	goNextLevel = false;
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


States GameState::m_currentState = Menu;
GameState* GameState::s_unique_instance = nullptr;