#pragma once
#include "Manager.h"
#include "ParticleSystem.h"
#include "GameState.h"

class ParticleManager: public Manager<ParticleManager>
{
	GameState* m_state= nullptr;
	ParticleSystem* m_enemyKilledFx = nullptr;
	ParticleSystem* m_playerBlood = nullptr;

public:
	ParticleManager() {};
	~ParticleManager();
	void init();
	void draw();
	void threadUpdate(const float& dt);
	void onEnemyDied(const float& posX, const float& posY);
	void onPlayerMoved(const float& posX, const float& posY);
	void onPlayerHurt();
};