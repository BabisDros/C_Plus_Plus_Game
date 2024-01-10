#include "ParticleManager.h"
#include "CallbackManager.h"
#include "GameState.h"

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
	CallbackManager::getInstance()->m_enemyDied.addTwoArgActionCallback(std::bind(&ParticleManager::onEnemyDied, this, std::placeholders::_1, std::placeholders::_2));
}

void ParticleManager::draw()
{
	m_enemyKilledFx->draw();	
}

void ParticleManager::update(float dt)
{	
	m_enemyKilledFx->update(dt);
}

void ParticleManager::onEnemyDied(const float posX, const float posY)
{
	m_enemyKilledFx->followHolderGameobject(posX, posY);
	m_enemyKilledFx->init();
}


