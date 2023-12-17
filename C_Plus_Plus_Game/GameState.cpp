#include "GameState.h"
#include "Level.h"
#include "Player.h"

GameState::GameState()
{
}

void GameState::init()
{
	m_current_level = new Level();
	m_current_level->init();

	m_player = new Player("Player");
	m_player->init();

	graphics::preloadBitmaps(getAssetDir()); // preload assets
	//graphics::setFont(m_asset_path + "path");		adds font
}

void GameState::draw()
{
	if (!m_current_level) return;

	m_current_level->draw();
}

void GameState::update(float dt)
{
	if (dt > 500) return;	// it been too long since last frame


	if (!m_current_level) return;

	m_current_level->update(dt);
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

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}



GameState* GameState::m_unique_instance = nullptr;