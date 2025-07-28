#pragma once
#include "Singleton.h"
class MusicManager: public Singleton<MusicManager>
{
	class GameState* m_state;
	bool m_playedWinSound = false;
public:
	bool m_play_door_sound = false;
	bool m_playing_door_sound = false;
	bool m_playedLoseSound = false;
	bool m_playing_music = false;
	void playMusic();
	void playWinSound();
	void playLoseSound();
	void playDoorSound();
	void init();
};