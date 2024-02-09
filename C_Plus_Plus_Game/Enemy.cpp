#pragma once
#include "Enemy.h"
#include "util.h"
#include "Level.h"
#include "Player.h"
#include "CallbackManager.h"

void Enemy::init()
{
	if (m_stick_to_wall == 3) m_pos_y += (1 - m_height) * 0.5f;	// Currently only done for either top or bottom

	m_homebase_x = m_pos_x;
	m_homebase_y = m_pos_y;
	setCustomBrushProperties(&m_brush, 1.0f, 0.0f, m_state->getFullAssetPath(*m_texture)); //"enemy.png"

	graphics::Brush fireball;
	setCustomBrushProperties(&fireball, 1.0f, 0.0f, m_state->getFullAssetPath("efecto_fuego_00030.png"));
	m_projectile.setBrush(fireball);
	m_projectile.setParentDirection(m_lookingDirection);
	if (m_body_damage) m_bodyDamage.setPosition(m_pos_x, m_pos_y, m_width, m_height);
	m_bodyDamage.setActive(true);
	if (m_rangedAttack) { m_projectile.m_width = 1.2f; m_projectile.m_height = 0.8f; m_projectile.m_diesOnTouch = true; }
}

void Enemy::draw()
{
	float angle = 0;
	if (m_stick_to_wall == 1) angle = 180.f;
	graphics::setOrientation(angle);

	m_mirrored = m_lookingDirection == -1;
	if (m_mirrored) graphics::setScale(-1.f, 1.f); //mirrors image
	//! -0.5f MUST be gone
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_brush);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_id);
	}
	m_projectile.draw();
	m_healthUi->draw();
}

void Enemy::update(const float& dt)
{
	m_collisingLR = false;
	checkCollision(m_state->getLevel()->getBlocks());
	checkCollision(m_state->getLevel()->getDestructibleObjects());
	movement(dt);
	attack(dt);
	m_healthUi->setPosition(m_pos_x, m_pos_y);
	if (m_throwProjectile.isRunning())
	{
		graphics::Brush fireball;
		fireball.outline_opacity = 0;
		float dif = *m_state->getPausableClock() - m_throwProjectile.getStartTime();

		int index = (int)(25 * dif) % (*m_state->getLevel()->getFireballSprites()).size();
		fireball.texture = m_state->getFullAssetPath((*m_state->getLevel()->getFireballSprites()).at(index));
		// at 32 fireball starts shrinking texture wise
		if (index > 32) m_projectile.m_width = 1.2 - 1.2 * (index - 32) / ((*m_state->getLevel()->getFireballSprites()).size() - 32);
		m_projectile.setBrush(fireball);
	}
}	

void Enemy::destroy()
{
	CallbackManager::getInstance()->m_pointsChanged.trigger(m_points,false);
	CallbackManager::getInstance()->m_enemyDied.trigger(m_pos_x,m_pos_y);
	setActive(false);
}


void Enemy::movement(const float& dt)
{
	const float& delta_time = dt / 1000.f;
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

void Enemy::movementStaticX(const float& dt)
{
	if (fabs(m_pos_x - m_homebase_x + dt * m_vx) > m_movement_range_x)
	{
		m_lookingDirection *= -1;
		m_vx = 0;
	}
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

void Enemy::movementStaticY(const float& dt)
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

void Enemy::movementDynamic(const float& dt)
{
	if (fabs(m_state->getPlayer()->m_pos_x - m_homebase_x) > m_movement_range_x + 1)	// if too far from player, do not attempt to chase
	{
		movementStaticX(dt);
	}
	else
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

		m_vx = std::min(m_max_velocity, m_vx + dt * move * m_accel_horizontal);
		m_vx = std::max(-m_max_velocity, m_vx);
		move = dt * m_vx;
		if (!canFollow || fabs(m_pos_x - m_homebase_x + move) > m_movement_range_x)  m_vx = 0;	// If it will get him outside of territory, stop
		m_pos_x += dt * m_vx;
	}
}

void Enemy::attack(const float& delta_time)
{
	const float& dt = delta_time / 1000.f;
	if (m_rangedAttack) rangedAttack(dt);
	if (m_body_damage) 
	{
		m_bodyDamage.setPosition(m_pos_x, m_pos_y, m_width, m_height);
		m_bodyDamage.update(dt);
	}
}

void Enemy::rangedAttack(const float& dt)
{
	m_projectile.update(dt);
	if (!m_throwProjectile.isRunning())
	{
		m_projectile.m_canMove = true;
		m_projectile.m_width = 1.2;

		m_projectile.setActive(true);
		m_throwProjectile.setStartTime(*m_state->getPausableClock());
		m_projectile.setPosition(m_pos_x, m_pos_y, m_projectile.m_width, m_projectile.m_height);
		m_projectile_direction = m_lookingDirection;
		m_projectile.m_draw_direction = m_projectile_direction;
		if (fabs(m_state->getPlayer()->m_pos_x - m_pos_x) < 25 && fabs(m_state->getPlayer()->m_pos_y - m_pos_y) < 15) // not too far
		{
			graphics::playSound("music\\enemy_projectile.wav", 0.05f);
		}
	}

	if (m_throwProjectile.isRunning())
	{	
		if (m_throwProjectile.getElapsedTime() < m_throwProjectile.getDuration())
		{
			if (m_projectile.m_canMove)
			{
				float move = m_projectile_direction;
				m_projectile_vx = std::min(m_projectile_max_velocity, m_projectile_vx + dt * move * m_projectile_accel_horizontal);
				m_projectile_vx = std::max(-m_projectile_max_velocity, m_projectile_vx);
				float distance = dt * m_projectile_vx;
				m_projectile.setPosition(m_projectile.m_pos_x + distance, m_projectile.m_pos_y, m_projectile.m_width, m_projectile.m_height); 
			}
			else
			{
				m_projectile.setActive(false);
			}
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
		accel = m_accel_vertical * 2.f;
	}

	if (m_jumpAbility.isRunning())
	{
		if ((m_jumpAbility.getElapsedTime() >= m_jumpAbility.getCooldown()) ||
			(m_collisingLR && m_jumpAbility.getElapsedTime() > 0.5f))	// get unstuck faster
		{
			m_jumpAbility.setStartTime(0.f);
		}
	}
	return accel;
}

