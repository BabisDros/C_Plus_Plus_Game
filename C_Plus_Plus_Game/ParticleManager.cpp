#include "ParticleManager.h"
#include "CallbackManager.h"
#include "GameState.h"
#include <iostream>

ParticleManager* ParticleManager::s_unique_instance = nullptr;
//Handles effects with particle systems
ParticleManager* ParticleManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new ParticleManager();
	}
	return s_unique_instance;
}

void ParticleManager::init()
{
	m_state = GameState::getInstance();

	m_enemyKilledFx = new ParticleSystem(5, 2, 1, 1, 0.5f, 1.5f, 0.5f, m_state->getFullAssetPath("smoke3.png"), 10.0f, 2.0f, 0.0f, 5.0f, 0.4f, 0.f, 0.f, 0.f);
	CallbackManager::getInstance()->m_enemyDied.addArgActionCallback(std::bind(&ParticleManager::onEnemyDied, this, std::placeholders::_1, std::placeholders::_2));
	
	//pos x and y are temporar
	m_playerBlood= new ParticleSystem(8, 20, 1, 1, 0.1f, 0.2f, 0.8f, m_state->getFullAssetPath("blood.png"), 10.f, 2.f, 5.f, 5.f, 0.4f);
	CallbackManager::getInstance()->m_playHurtFx.addArgActionCallback(std::bind(&ParticleManager::onPlayerHurt, this));
	CallbackManager::getInstance()->m_playerMoved.addArgActionCallback(std::bind(&ParticleManager::onPlayerMoved, this, std::placeholders::_1, std::placeholders::_2));
}

void ParticleManager::draw()
{
	
	m_enemyKilledFx->draw();
	m_playerBlood->draw();
}

void ParticleManager::threadUpdate(float dt)
{
	m_enemyKilledFx->update(dt);
	m_playerBlood->update(dt);
}
//because the enemy dies and stops moving, we can update m_enemyKilledFx position once
void ParticleManager::onEnemyDied(const float posX, const float posY)
{
	m_enemyKilledFx->followGameobject(posX, posY);
	m_enemyKilledFx->init();
}

//because player moves when is damaged, we update the m_playerBlood effect in every movement 
void ParticleManager::onPlayerMoved(const float posX, const float posY)
{
	m_playerBlood->followGameobject(posX, posY);
}

void ParticleManager::onPlayerHurt()
{
	m_playerBlood->init();
}


