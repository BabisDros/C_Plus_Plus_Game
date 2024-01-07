#pragma once

class MusicManager
{
	class GameState* m_state;
	static MusicManager* s_unique_instance;
public:
	bool m_playing_music = false;
	static MusicManager* getInstance();
	void playMusic();
	void init();
};