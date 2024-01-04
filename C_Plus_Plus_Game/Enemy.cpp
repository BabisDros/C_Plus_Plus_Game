#include "Enemy.h"
#include "util.h"
#include "GameState.h"
#include <iostream>
#include "Level.h"
#include "Player.h"
void Enemy::init()
{
	m_homebase_x = m_pos_x;
	m_homebase_y = m_pos_y;
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
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_id);
	}
}

void Enemy::update(float dt)
{
	checkCollision(m_state->getLevel()->getBlocks());
//	std::vector<Player*> temp(1,m_state->getPlayer());	//Attempt to interact with player
//	checkCollision(temp);
	movement(dt);
}

void Enemy::destroy()
{
}

void Enemy::instantiateParticles()
{
}

void Enemy::movement(float dt)
{
	float delta_time = dt / 1000.f;
	switch (m_movement_type)
	{
		case 1: 
			movementDynamic(delta_time);
			break;
		case 2:
			movementStaticX(delta_time);
			break;
		case 3:
			movementStaticY(delta_time);
			break;
	}
}

void Enemy::movementStaticX(float dt)
{
	if (fabs(m_pos_x - m_homebase_x + dt * m_vx) > m_movement_range_x)
	{
		m_lookingDirection *= -1;
		m_vx = 0;
	}
	m_mirrored = m_lookingDirection == 1;
	m_vx = std::min(m_max_velocity, m_vx + dt * m_lookingDirection * m_accel_horizontal);
	m_vx = std::max(-m_max_velocity, m_vx);
	m_pos_x += dt * m_vx;

	for (auto itr : m_state->getLevel()->getBlocks())	//check if we found wall after movement
	{
		if (intersect(*itr))
		{
			m_lookingDirection *= -1;
			break;
		}
	}
}

void Enemy::movementStaticY(float dt)
{
	if (fabs(m_pos_y - m_homebase_y + dt * m_vy) > m_movement_range_y)
	{
		m_lookingDirection *= -1;
		m_vy = 0;
	}
	m_vy = std::min(m_max_velocity, m_vy + dt * m_lookingDirection * m_accel_vertical);
	m_vy = std::max(-m_max_velocity, m_vy);
	m_pos_y += dt * m_vy;

	for (auto itr : m_state->getLevel()->getBlocks())	//check if we found wall after movement
	{
		if (intersect(*itr))
		{
			m_lookingDirection *= -1;
			break;
		}
	}
}

void Enemy::movementDynamic(float dt)
{
	m_vy += dt * m_gravity;
	m_pos_y += dt * m_vy;
	float move = 0.f;
	bool canFollow = true;
	if (m_state->getPlayer()->m_pos_x + m_state->getPlayer()->m_width / 2.f < m_pos_x - m_width / 2.f)
	{
		m_mirrored = false;
		move = -1;
	}
	else if (m_state->getPlayer()->m_pos_x - m_state->getPlayer()->m_width / 2.f > m_pos_x + m_width / 2.f)
	{
		m_mirrored = true;
		move = 1;
	}
	else canFollow = false;

	m_vx = std::min(m_max_velocity, m_vx + dt * move * m_accel_horizontal);
	m_vx = std::max(-m_max_velocity, m_vx);
	move = dt * m_vx;
	if (!canFollow || fabs(m_pos_x - m_homebase_x + move) > m_movement_range_x)  m_vx = 0;	// If it will get him outside of territory, stop
	m_pos_x += dt * m_vx;
}
