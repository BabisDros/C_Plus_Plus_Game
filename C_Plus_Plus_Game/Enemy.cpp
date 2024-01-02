#include "Enemy.h"
#include "util.h"
#include "GameState.h"
#include <iostream>
#include "Level.h"
#include "Player.h"
void Enemy::init()
{
	m_pos_x = -8.f;
	m_pos_y = 8.f;
	setCustomBrushProperties(&m_brush, 1.0f, 0.0f, m_state->getFullAssetPath("temp_enemy2.png"));

	m_initialHealth = m_currentHealth = 100;
}

void Enemy::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image
	//! -0.5f MUST be gone
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 2.0f, 2.0f, m_brush);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height);
	}
}

void Enemy::update(float dt)
{
	checkCollision(m_state->getLevel()->getBlocks());
//	std::vector<Player*> temp(1,m_state->getPlayer());	Attempt to interact with player
//	checkCollision(temp);
	float delta_time = dt / 1000.0f;
	m_vy += delta_time * m_gravity;
	m_pos_y += delta_time * m_vy;
}

void Enemy::destroy()
{
}

void Enemy::instantiateParticles()
{
}
