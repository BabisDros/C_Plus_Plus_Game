#pragma once
#include "MusicManager.h"
#include "LevelManager.h"
#include "UIManager.h"
#include "GameState.h"
#include "CallbackManager.h"
#include "ParticleManager.h"
#include "Level.h"
#include "Player.h"
#include <thread>
#include <chrono>
#include <iostream>



States GameState::m_currentState = Menu;
GameState* GameState::s_unique_instance = nullptr;

GameState::GameState()
{
}

void GameState::init()
{	
	CallbackManager::getInstance()->m_pointsChanged.addArgActionCallback(std::bind(&GameState::onPointsCollected, this, std::placeholders::_1));
	CallbackManager::getInstance()->m_playerLivesChanged.addArgActionCallback(std::bind(&GameState::onPlayerLivesChanged, this, std::placeholders::_1));
	UIManager::getInstance()->init();
	ParticleManager::getInstance()->init();
	LevelManager::getInstance()->init();	
	MusicManager::getInstance()->init();
	graphics::preloadBitmaps(getAssetDir()); //? preload assets
	//graphics::setFont(m_asset_path + "path");		//?	adds font
}

void GameState::draw()
{		
	if (m_current_level!=nullptr)
	{
		m_current_level->draw();
	}

	UIManager::getInstance()->draw();
}

void GameState::update(float dt)
{
	//std::cout << "Thread 2 " << dt << std::endl;
	if (dt > 200) return;	//? it been too long since last frame
	/* fixes screenshaking, basically reducing frames, no issues currently, no need to enable it
	float sleep_time = std::max(0.0f, 17.0f - dt);
	std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));*/ 
	UIManager::getInstance()->update(dt);
	LevelManager::getInstance()->update(dt);
	ParticleManager::getInstance()->threadUpdate(dt);
	handleStates();
	if (!m_current_level) return;

	if (!m_suspendExecution)
	{
		m_current_level->update(dt);
		m_pausableClock += graphics::getDeltaTime()/1000;
	}
	
	enable(m_debugging, m_debugging_held, graphics::getKeyState(graphics::SCANCODE_0));
	enable(m_pauseButtonPressed, m_paused_held, graphics::getKeyState(graphics::SCANCODE_P));
	showFPS();
	if (m_goNextLevel) LevelManager::getInstance()->nextLevel();
	if (m_currentState == InGame && !MusicManager::getInstance()->m_playing_music) MusicManager::getInstance()->playMusic();
}
/// sto


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
	deletePlayer();
	if (m_current_level)
	{
		delete m_current_level;
	}
}

void GameState::handleStates()
{
    if (m_currentState == Menu)
    {      
        if (graphics::getKeyState(graphics::SCANCODE_N) )
        {
            LevelManager::getInstance()->nextLevel();
            m_currentState = States::InGame;
        }
        else if (graphics::getKeyState(graphics::SCANCODE_L))
        {
            LevelManager::getInstance()->m_loadingFile = true;
            LevelManager::getInstance()->loadSaveFile();
            LevelManager::getInstance()->m_loadingFile = false;
            m_currentState = States::InGame;
        }     
    }
	else if (m_currentState == InGame)
	{
		if (m_pauseButtonPressed)
		{
			m_currentState = States::Paused;
			m_suspendExecution = true;
		}
		
	}
	else if (m_currentState == Paused)
    {      
        if (graphics::getKeyState(graphics::SCANCODE_R))
        {
			LevelManager::getInstance()->m_restart = true;
        }  

		if (!m_pauseButtonPressed)
		{
			m_currentState = States::InGame;
			m_suspendExecution = false;
		}
    }

	else if (m_currentState == Lose)
	{
		m_suspendExecution = true;
		MusicManager::getInstance()->playLoseSound();
		if (graphics::getKeyState(graphics::SCANCODE_R))
		{
			LevelManager::getInstance()->m_level_counter = 0;	//go back to first level and reset score
			m_points = 0;	
			UIManager::getInstance()->onPointsChanged();
			MusicManager::getInstance()->m_playedLoseSound = false;
			LevelManager::getInstance()->m_restart = true;
			CallbackManager::getInstance()->m_playerLivesChanged.trigger(m_initialLives);
		}
	}
}
void GameState::setState(States state)
{
	m_currentState = state;
}

void GameState::deletePlayer() const
{
	if (m_player)
	{
		delete m_player;
	}
}

void GameState::showFPS()
{
	float currentTime = graphics::getGlobalTime() / 1000;
	if (currentTime > m_time)
	{
		UIManager::getInstance()->m_fps = m_fps;
		m_fps = 0;
		++m_time;
	}
	else
	{
		++m_fps;
	}
}

void GameState::onPointsCollected(int points)
{
	m_points += points;
}

void GameState::onPlayerLivesChanged(int life)
{
	m_lives += life;
}

States& GameState::getCurrentState()
{
	return m_currentState;
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

int GameState::getInitialLives() const
{
	return m_initialLives;
}

int GameState::getInitialHealth() const
{
	return m_initialHealth;
}

