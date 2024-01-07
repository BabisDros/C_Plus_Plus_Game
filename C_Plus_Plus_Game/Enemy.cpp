#pragma once
#include "Enemy.h"
#include "util.h"
#include "GameState.h"
#include <iostream>
#include "Level.h"
#include "Player.h"

void Enemy::init()
{
	if (m_stick_to_wall = 2) m_pos_x += (1 - m_width) * 0.5f;	// 0.5 is based on level zoom
	if (m_stick_to_wall = 3) m_pos_y += (1 - m_height);

	//if (m_movement_type = 0) { m_movement_range_x = m_movement_range_x = 0; }
	m_homebase_x = m_pos_x;
	m_homebase_y = m_pos_y;
	setCustomBrushProperties(&m_brush, 1.0f, 0.0f, m_state->getFullAssetPath(*m_texture)); //"temp_enemy2.png"

	m_initialHealth = m_currentHealth = 100;

	graphics::Brush slash;
	setCustomBrushProperties(&slash, 1.0f, 0.0f, m_state->getFullAssetPath("slashFx.png"));
	m_projectile.setBrush(slash);

	m_projectile.m_parentDirection = &m_lookingDirection;
}

void Enemy::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image
	//! -0.5f MUST be gone
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_brush);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_id);
	}
	m_projectile.draw();
}

void Enemy::update(float dt)
{
	checkCollision(m_state->getLevel()->getBlocks());
//	std::vector<Player*> temp(1,m_state->getPlayer());	//Attempt to interact with player
//	checkCollision(temp);
	movement(dt);
	attack(dt);
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
	if (m_canJump)
	{
		m_vy -= jump();
		m_vy = std::min(m_max_velocity_jump, m_vy);
		m_vy = std::max(-m_max_velocity_jump, m_vy);

		m_vy += delta_time * m_gravity;
		m_pos_y += delta_time * m_vy;
	}
}

void Enemy::movementStaticX(float dt)
{
	if (fabs(m_pos_x - m_homebase_x + dt * m_vx) > m_movement_range_x)
	{
		m_lookingDirection *= -1;
		m_vx = 0;
	}
	m_mirrored = m_lookingDirection == -1;
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
		m_direction_y *= -1;
		m_vy = 0;
	}
	m_vy = std::min(m_max_velocity, m_vy + dt * m_direction_y * m_accel_vertical);
	m_vy = std::max(-m_max_velocity, m_vy);
	m_pos_y += dt * m_vy;

	for (auto itr : m_state->getLevel()->getBlocks())	//check if we found wall after movement
	{
		if (intersect(*itr))
		{
			m_direction_y *= -1;
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
		m_lookingDirection = -1;
		move = -1;
	}
	else if (m_state->getPlayer()->m_pos_x - m_state->getPlayer()->m_width / 2.f > m_pos_x + m_width / 2.f)
	{
		m_lookingDirection = 1;
		move = 1;
	}
	else canFollow = false;

	m_mirrored = m_lookingDirection == -1;
	m_vx = std::min(m_max_velocity, m_vx + dt * move * m_accel_horizontal);
	m_vx = std::max(-m_max_velocity, m_vx);
	move = dt * m_vx;
	if (!canFollow || fabs(m_pos_x - m_homebase_x + move) > m_movement_range_x)  m_vx = 0;	// If it will get him outside of territory, stop
	m_pos_x += dt * m_vx;
}

void Enemy::attack(float delta_time)
{
	float dt = delta_time / 1000.f;
	if (m_rangedAttack) rangedAttack(dt);
}

void Enemy::rangedAttack(float dt)
{
	m_projectile.update(dt);
	if (!m_throwProjectile.isRunning())
	{
		m_projectile.setActive(true);
		m_throwProjectile.setStartTime(*m_state->getPausableClock());
		m_projectile.setPosition(m_pos_x, m_pos_y, 0.8f, 0.8f);
		m_projectile_direction = m_lookingDirection;
	}

	if (m_throwProjectile.isRunning())
	{
	
		if (m_throwProjectile.getElapsedTime() < m_throwProjectile.getDuration())
		{
			float move = m_projectile_direction;
			m_projectile_vx = std::min(m_projectile_max_velocity, m_projectile_vx + dt * move * m_projectile_accel_horizontal);
			m_projectile_vx = std::max(-m_projectile_max_velocity, m_projectile_vx);
			float distance = dt * m_projectile_vx;
			m_projectile.setPosition(m_projectile.m_pos_x + distance, m_projectile.m_pos_y, 0.8f, 0.8f);
		}
		else if (m_projectile.isActive())
		{
			m_projectile.setActive(false);
		}
	
		m_throwProjectile.resetIfCooldownExpired();
		
	}
}

float Enemy::jump()
{
	float accel = 0;
	if (m_vy == 0.0f && !m_jumpAbility.isRunning())
	{
		m_jumpAbility.setStartTime(*m_state->getPausableClock());
		accel = m_accel_vertical * 0.02f;//? not delta_time! Burst [Papaioannou comment]
	}

	if (m_jumpAbility.isRunning())
	{
		if (m_jumpAbility.getElapsedTime() >= m_jumpAbility.getCooldown())
		{
			m_jumpAbility.setStartTime(0.f);
		}
	}
	return accel;
}
