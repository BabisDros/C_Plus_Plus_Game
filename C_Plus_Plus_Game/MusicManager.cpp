#include "MusicManager.h"
#include "GameState.h"

void MusicManager::init()
{
	m_state = GameState::getInstance();
}

void MusicManager::playMusic()
{
	if (!m_playing_door_sound)
	{
		graphics::playMusic("music\\Journey.wav", 0.05); //"music\\01. Enter Hallownest.mp3"
		m_playing_music = true;
	}
}

void MusicManager::playWinSound()
{
	if (!m_playedWinSound)
	{
		graphics::playMusic("music\\Success_3.wav", 0.05, false);
		m_playedWinSound = true;
		m_playing_music = false;
	}
}
void MusicManager::playLoseSound()
{
	if (!m_playedLoseSound)
	{
		graphics::playMusic("music\\Lost_sound_5.wav", 0.1, false);
		m_playedLoseSound = true;
		m_playing_music = false;
	}
}

void MusicManager::playDoorSound()
{
	if (m_play_door_sound)
	{
		graphics::playMusic("music\\open close door.wav", 0.1f);
		m_play_door_sound = false;
		m_playing_music = false;
		m_playing_door_sound = true;
	}
}
