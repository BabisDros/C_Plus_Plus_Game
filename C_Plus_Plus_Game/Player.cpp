#pragma once
#include "Player.h"
#include "GameState.h"
#include "util.h"
#include <iostream>
#include "Level.h"
#include "CallbackManager.h"
#include <filesystem> // to read sprites for animation 
void Player::init()
{
	m_pos_x = m_state->getLevel()->m_player_start_x;
	m_pos_y = m_state->getLevel()->m_player_start_y;

	setCustomBrushProperties(&m_brush, 1.0f, 0.0f, m_state->getFullAssetPath("Idle1.png"));

	graphics::Brush slash;
	setCustomBrushProperties(&slash, 1.0f, 0.0f, m_state->getFullAssetPath("slashFx.png"));
	m_damageBox.setBrush(slash);

	m_damageBox.m_parentDirection = &m_lookingDirection;
	//trigger callbackmanager to display health value
	CallbackManager::getInstance()->m_playerIsDamaged.trigger(IDestructible::m_initialHealth, IDestructible::m_currentHealth);
//	m_initialHealth = m_currentHealth = 100; // Was reseting hp between levels

	for (const auto& entry : std::filesystem::directory_iterator(m_state->getFullAssetPath("Character Sprites V2\\Run")))
	{
		sprites.push_back(entry.path().u8string());
	}

}

void Player::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image
																						//! -0.5f MUST be gone
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.2f, 1.0f, m_brush);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_id);
	}	
	m_damageBox.draw();
}

void Player::update(float dt)
{
	checkCollision(m_state->getLevel()->getBlocks());
	checkCollision(m_state->getLevel()->getDynamicObjects());
	m_damageBox.update(dt);
	float delta_time = dt / 1000.0f;

	m_state->enable(m_dev_fly, m_dev_fly_held, graphics::getKeyState(graphics::SCANCODE_MINUS));
	if (m_dev_fly) fly(delta_time);
	else movement(delta_time);

	if (m_pos_y > m_state->getCanvasHeight() + 2) //? is in void
	{
		m_pos_x = m_state->getLevel()->m_player_start_x; 
		m_pos_y = m_state->getLevel()->m_player_start_y;
	}

	cameraOffsetX(0.4f, 0.6f);		//! Preferably make it a single function {with array, enum?}, only if it isn't complex
	cameraOffsetY(0.3f, 0.7f);

	dash(delta_time);
	slash(delta_time);
	tempTimer += (10.f * delta_time);
	m_brush.texture = sprites.at((int) (tempTimer) % sprites.size());
}

void Player::destroy()
{
	setActive(false);
}

void Player::instantiateParticles()
{
}

void Player::movement(float delta_time)
{
	float move = 0;

	if (graphics::getKeyState(graphics::SCANCODE_A))	//? movement
	{
		move = -1.0f;
		m_mirrored = true;
		m_lookingDirection = -1;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		move = 1.0f;
		m_mirrored = false;
		m_lookingDirection = 1;
	}

	if ((move > 1 && m_vx < 0) || move < 1 && m_vx > 0) m_vx = 0; // guaranteed to reset speed when changing direction

	if (graphics::getKeyState(graphics::SCANCODE_D) ^ graphics::getKeyState(graphics::SCANCODE_A)) //? insta stop
	{
		m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
		m_vx = std::max(-m_max_velocity, m_vx);	
	}
	else
	{
		m_vx = 0.f;
	}

	m_pos_x += delta_time * m_vx;

	m_vy -= jump();

	m_vy = std::min(m_max_velocity, m_vy);
	m_vy = std::max(-m_max_velocity, m_vy);
	
	m_vy += delta_time * m_gravity;	
	m_pos_y += delta_time * m_vy;
}

float Player::jump()
{
	float accel = 0;
	if (m_vy == 0.0f && graphics::getKeyState(graphics::SCANCODE_W) && !m_jumpAbility.isRunning())
	{
		m_jumpAbility.setStartTime(*m_state->getPausableClock());
		accel= m_accel_vertical * 0.02f;//? not delta_time! Burst [Papaioannou comment]
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

void Player::fly(float delta_time)
{
	const float velocity = 10.0f;
	if (graphics::getKeyState(graphics::SCANCODE_A))	//? movement
	{
		m_pos_x -= velocity * delta_time;
		m_mirrored = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		m_pos_x += velocity * delta_time;
		m_mirrored = false;
	}
	if (graphics::getKeyState(graphics::SCANCODE_W))
	{
		m_pos_y -= velocity * delta_time;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S))
	{
		m_pos_y += velocity * delta_time;
	}
}

void Player::dash(float delta_time)
{
	if (graphics::getKeyState(graphics::SCANCODE_F) && !m_dashAbility.isRunning())
	{
		m_dashAbility.setStartTime(*m_state->getPausableClock());
	}	
	
	if (m_dashAbility.isRunning())
	{	
		//dash has certain duration, otherwise character teleports to another position and creates bugs like passing through objects
		if (m_dashAbility.getElapsedTime() < m_dashAbility.getDuration())
		{
			m_vx = m_dashAbility.getSpeed() * m_lookingDirection;
			m_pos_x += delta_time * m_vx;
		}
		m_dashAbility.resetIfCooldownExpired();
	}
}

void Player::slash(float delta_time)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && !m_slashAbility.isRunning())
	{	
		m_damageBox.setActive(true);
		m_slashAbility.setStartTime(*m_state->getPausableClock());
	}
	if (m_slashAbility.isRunning())
	{
		if (m_slashAbility.getElapsedTime() < m_slashAbility.getDuration())
		{
			//extra offset corrected with the player's direction
			float lookingDirOffset = 0.5f * m_lookingDirection;
			//slash damagebox follows player
			m_damageBox.setPosition(m_pos_x + lookingDirOffset, m_pos_y , 0.8f, 0.8f);
			takeDamage(10);//TODO: Delete. It is for checking if CallbackManager works
		}
		else if(m_damageBox.isActive())
		{			
			m_damageBox.setActive(false);
		}

		m_slashAbility.resetIfCooldownExpired();
	}
}

void Player::takeDamage(const int& damage)
{
	//call base class to update health
	IDestructible::takeDamage(damage);

	//trigger
	CallbackManager::getInstance()->m_playerIsDamaged.trigger( IDestructible::m_initialHealth, IDestructible::m_currentHealth);
}


void Player::cameraOffsetX(float multiplier1, float multiplier2)
{															//? 0.5f is based on zoom done on background, check level draw/init
	if (m_pos_x >= m_state->getCanvasWidth()*(multiplier1 - 0.5f) && (m_pos_x <= m_state->getCanvasWidth() * (multiplier2 + 0.5f))) //? prevents going outside of background
	{
		if (m_pos_x + m_state->m_global_offset_x <= m_state->getCanvasWidth() * multiplier1)
		{
			m_state->m_global_offset_x = m_state->getCanvasWidth() * multiplier1 - m_pos_x;	
		}
		else if (m_pos_x + m_state->m_global_offset_x >= m_state->getCanvasWidth() * multiplier2)
		{
			m_state->m_global_offset_x = m_state->getCanvasWidth() * multiplier2 - m_pos_x;
		}
	}
	else if (m_pos_x < m_state->getCanvasWidth() * (multiplier1 - 0.5f))	//! Not needed for Y since there isn't currently anything higher
	{
		m_state->m_global_offset_x = m_state->getCanvasWidth() * 0.5f;
	}
	else if (m_pos_x > m_state->getCanvasWidth() * (multiplier2 + 0.5f))
	{
		m_state->m_global_offset_x = -m_state->getCanvasWidth() * 0.5f;
	}
}

void Player::cameraOffsetY(float multiplier1, float multiplier2)
{
	if (m_pos_y >= m_state->getCanvasHeight() * (multiplier1) && (m_pos_y <= m_state->getCanvasHeight() * (multiplier2))) //? prevents going outside of background
	{ 
		if (m_pos_y + m_state->m_global_offset_y <= m_state->getCanvasHeight() * multiplier1)
		{
			m_state->m_global_offset_y = m_state->getCanvasHeight() * multiplier1 - m_pos_y; 
		}
		else if (m_pos_y + m_state->m_global_offset_y >= m_state->getCanvasHeight() * multiplier2)
		{
			m_state->m_global_offset_y = m_state->getCanvasHeight() * multiplier2 - m_pos_y; 
		}
	}
}
