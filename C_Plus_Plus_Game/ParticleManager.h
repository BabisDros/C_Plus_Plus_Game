#pragma once
#include "ParticleSystem.h"
#include "GameState.h"

class ParticleManager
{
	static ParticleManager* s_unique_instance;
	GameState* m_state= nullptr;
	ParticleSystem* m_enemyKilledFx = nullptr;
	ParticleSystem* m_playerBlood = nullptr;

public:
	ParticleManager() {};
	~ParticleManager();
	void init();
	void draw();
	void threadUpdate(float dt);
	void onEnemyDied(const float posX, const float posY);
	void onPlayerMoved(const float posX, const float posY);
	void onPlayerHurt();
	static ParticleManager* getInstance();
};