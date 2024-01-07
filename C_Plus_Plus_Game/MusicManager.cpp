#include "MusicManager.h"
#include "sgg/graphics.h"
#include "GameState.h"

MusicManager* MusicManager::s_unique_instance = nullptr;

MusicManager* MusicManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new MusicManager();
	}
	return s_unique_instance;
}

void MusicManager::init()
{
	m_state = GameState::getInstance();
}

void MusicManager::playMusic()
{
	graphics::playMusic("music\\01. Enter Hallownest.mp3", 0.15);
	m_playing_music = true;
}
