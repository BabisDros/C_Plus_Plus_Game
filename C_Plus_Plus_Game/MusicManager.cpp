#include "MusicManager.h"
#include "GameState.h"

void MusicManager::init()
{
	m_state = GameState::getInstance();
}

void MusicManager::playMusic()
{
	if (!m_playing_door_sound && !m_playing_music)
	{
		graphics::playMusic("music\\Battle_Ready-Freepd_Bryan_Teoh.mp3", 0.05f);
		m_playing_music = true;
	}
}

void MusicManager::playWinSound()
{
	if (!m_playedWinSound)
	{
		graphics::playMusic("music\\456966_funwithsound_success-fanfare-trumpets.wav", 0.1f, false);
		m_playedWinSound = true;
		m_playing_music = false;
	}
}
void MusicManager::playLoseSound()
{
	if (!m_playedLoseSound)
	{	
		//if file does not play try format of 44100 Hz, 16 bit 
		graphics::playMusic("music\\space_horn_2-FilmCow.wav", 0.3f,false);
		
		m_playing_music = false;
		m_playedLoseSound = true;	
	}
}

void MusicManager::playDoorSound()
{
	if (m_play_door_sound)
	{
		graphics::playMusic("music\\door_open_5-FilmCow.wav", 0.1f, false);
		m_play_door_sound = false;
		m_playing_music = false;
		m_playing_door_sound = true;
	}
}
