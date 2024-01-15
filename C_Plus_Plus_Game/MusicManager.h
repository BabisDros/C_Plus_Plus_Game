#pragma once

class MusicManager
{
	class GameState* m_state;
	static MusicManager* s_unique_instance;
	bool m_playedWinSound = false;
public:
	bool m_playedLoseSound = false;
	bool m_playing_music = false;
	static MusicManager* getInstance();
	void playMusic();
	void playWinSound();
	void playLoseSound();
	void init();
};