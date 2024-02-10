#pragma once
#include "MusicManager.h"
#include "LevelManager.h"
#include "UIManager.h"
#include "GameState.h"
#include "CallbackManager.h"
#include "ParticleManager.h"
#include "Level.h"
#include <filesystem> // to read sprites for animation 

States GameState::m_currentState = Menu;
GameState* GameState::s_unique_instance = nullptr;

GameState::GameState()
{
}

void GameState::init()
{	
	CallbackManager::getInstance()->m_pointsChanged.addArgActionCallback(std::bind(&GameState::onPointsCollected, this, std::placeholders::_1, std::placeholders::_2));
	CallbackManager::getInstance()->m_playerLivesChanged.addArgActionCallback(std::bind(&GameState::onPlayerLivesChanged, this, std::placeholders::_1, std::placeholders::_2));
	UIManager::getInstance()->init();
	ParticleManager::getInstance()->init();
	LevelManager::getInstance()->init();	
	MusicManager::getInstance()->init();
	// read and save animation sprites
	readSprites("Character Sprites V2\\Walk", m_sprites_walking);
	readSprites("Character Sprites V2\\Idle", m_sprites_idle);
	readSprites("Character Sprites V2\\Attack_B", m_sprites_attacking);
	readSprites("Character Sprites V2\\Jump", m_sprites_jumping);
	readSprites("Character Sprites V2\\Run", m_sprites_dashing);
	readSprites("Character Sprites V2\\Hurt", m_sprites_hurt);
	readSprites("fireball", m_fireball_sprites);
	readSprites("level cutscene", m_door_sprites);

	for (auto& p : std::filesystem::recursive_directory_iterator(getAssetDir())) // preload all assets
		if (p.is_directory()) graphics::preloadBitmaps(p.path().string());
			
}

void GameState::draw()
{		
	if (m_current_level!=nullptr)
	{
		m_current_level->draw();
	}
	UIManager::getInstance()->draw();
}

void GameState::update(const float& dt)
{
	if (dt > 200) return;	//? it been too long since last frame
	/* fixes screenshaking, basically reducing frames, no issues currently, no need to enable it
	float sleep_time = std::max(0.0f, 17.0f - dt);
	std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));*/ 
	UIManager::getInstance()->update(dt);
	LevelManager::getInstance()->update(dt);
	handleStates();
	if (!m_current_level) return;
	ParticleManager::getInstance()->threadUpdate(dt);

	if (!m_suspendExecution)
	{
		m_current_level->update(dt);
		m_pausableClock += graphics::getDeltaTime()/1000;
	}
	
	enable(m_debugging, m_debugging_held, graphics::getKeyState(graphics::SCANCODE_8));
	enable(m_pauseButtonPressed, m_paused_held, graphics::getKeyState(graphics::SCANCODE_P));
	showFPS();
	if (m_goNextLevel)
	{
		LevelManager::getInstance()->levelEndCutscene();
		if (LevelManager::getInstance()->m_cutscene_ended) LevelManager::getInstance()->nextLevel(); // level sequence animation
	}
	if (m_currentState == InGame) MusicManager::getInstance()->playMusic();
}

GameState::~GameState()
{
	deletePlayer();
	if (m_current_level)
	{
		delete m_current_level;
	}
	LevelManager::getInstance()->delptr();
	CallbackManager::getInstance()->delptr();
	UIManager::getInstance()->delptr();
	ParticleManager::getInstance()->delptr();
	MusicManager::getInstance()->delptr();
}

void GameState::handleStates()
{
    if (m_currentState == Menu)
    {      
        if (graphics::getKeyState(graphics::SCANCODE_N))
        {
            LevelManager::getInstance()->nextLevel();
            m_currentState = States::InGame;
			m_time = graphics::getGlobalTime() / 1000 - 0.5f;	//reduce delay till showing fps
        }
        else if (graphics::getKeyState(graphics::SCANCODE_L))
        {
            LevelManager::getInstance()->m_loadingFile = true;
            LevelManager::getInstance()->loadSavedFile();
            LevelManager::getInstance()->m_loadingFile = false;
            m_currentState = States::InGame;
			m_time = graphics::getGlobalTime() / 1000 - 0.5f;	//reduce delay till showing fps
        }
		else if (graphics::getKeyState(graphics::SCANCODE_I))
		{
			m_currentState = States::Help;
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
			CallbackManager::getInstance()->m_pointsChanged.trigger(0,true);
			MusicManager::getInstance()->m_playedLoseSound = false;
			LevelManager::getInstance()->setRestartAfterDeath(true);
			CallbackManager::getInstance()->m_playerLivesChanged.trigger(m_initialLives,true);
		}
	}
	else if (m_currentState == Win)
	{
		std::filesystem::remove("data\\save file.txt");
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

void GameState::onPointsCollected(int points,bool set)
{
	if (set)
	{
		m_points = points;
	}
	else
	{
		m_points += points;
	}
}

void GameState::onPlayerLivesChanged(int life,bool set)
{
	if (set)
	{
		m_lives = life;
	}
	else
	{
		m_lives += life;
	}
}

States& GameState::getCurrentState() const
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

std::string GameState::getAssetDir() const
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

// Used for Level data	
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


Player* GameState::getPlayer() const
{
	return m_player;
}

void GameState::setPlayer(Player* player)
{
	m_player = player;
}

Level* GameState::getLevel() const
{
	return m_current_level;
}

void GameState::setLevel(Level* level)
{
	m_current_level = level;
}

void GameState::readSprites(std::string folder, std::vector<std::string>& myVec)
{
	for (const auto& entry : std::filesystem::directory_iterator(getFullAssetPath(folder)))
	{
		myVec.push_back(entry.path().u8string().replace(0, getFullAssetPath("").size(), ""));
	}
}

int GameState::getLastHealth() const
{
	return playersLevelStartingHealth;
}

void GameState::setLastHealth(const int& health)
{
	playersLevelStartingHealth = health;
}