#pragma once
#include "ParticleSystem.h"

class ParticleManager
{
	static ParticleManager* s_unique_instance;
	class GameState* m_state;
	ParticleSystem* m_enemyKilledFx = nullptr;
	ParticleSystem* m_playerBlood = nullptr;

public:
	static ParticleManager* getInstance();
	void init();
	void draw();
	void threadUpdate(float dt);
	void onEnemyDied(const float posX, const float posY);
	void onPlayerMoved(const float posX, const float posY);
	void onPlayerHurt();
};