#include "Player.h"
#include "GameState.h"
#include "util.h"
#include <iostream>
#include "Level.h"

void Player::init()
{
//	m_pos_x = m_state->getCanvasWidth() / 4.0f; 
//	m_pos_y = m_state->getCanvasHeight() - 1.f;	//?? make initial var accesible

	m_pos_x = m_state->getLevel()->m_player_start_x;
	m_pos_y = m_state->getLevel()->m_player_start_y;

	setCustomBrushProperties(&m_brush, 1.0f, 0.0f, m_state->getFullAssetPath("Player\\Idle\\Idle1.png"));

	graphics::Brush slash;
	setCustomBrushProperties(&slash, 1.0f, 0.0f, m_state->getFullAssetPath("slashFx.png"));
	damageBox.setBrush(slash);

	damageBox.m_parentDirection = &m_lookingDirection;
	m_initialHealth = m_currentHealth = 100;

}

void Player::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image
																						//! -0.5f MUST be gone
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y-0.5f, 2.0f, 2.0f, m_brush);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height);
	}	
	damageBox.draw();
}

void Player::update(float dt)
{
	checkCollision(m_state->getLevel()->getBlocks());
	checkCollision(m_state->getLevel()->m_test_enemy);
	damageBox.update(dt);
	float delta_time = dt / 1000.0f;

	m_state->enable(m_dev_fly, m_dev_fly_held, graphics::getKeyState(graphics::SCANCODE_MINUS));
	if (m_dev_fly) fly(delta_time);
	else movement(delta_time);	//! player can currently hover below blocks if holding W, needs fix

	if (m_pos_y > m_state->getCanvasHeight() + 2) //? is in void
	{
		m_pos_x = m_state->getCanvasWidth() / 2.0f; //? centered
		m_pos_y = m_state->getCanvasHeight() / 2.0f; //! should be made to get to starting position
	}

	cameraOffsetX(0.4f, 0.6f);		//! Preferably make it a single function {with array, enum?}, only if it isn't complex
	cameraOffsetY(0.3f, 0.7f);

	dash(delta_time);
	slash(delta_time);
	//std::cout << "Player pos " << m_pos_x  << std::endl;
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

	if (graphics::getKeyState(graphics::SCANCODE_D) ^ graphics::getKeyState(graphics::SCANCODE_A)) //? insta stop
	{
		m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
		m_vx = std::max(-m_max_velocity, m_vx);
		//m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx)); //? slow down
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
	if (m_vy == 0.0f && !m_collidingUp && graphics::getKeyState(graphics::SCANCODE_W) && !jumpAbility.isRunning())
	{
		jumpAbility.setStartTime(m_state->m_pausableClock);
		accel= m_accel_vertical * 0.02f;//? not delta_time! Burst [Papaioannou comment]
	}
	
	if (jumpAbility.isRunning())
	{
		float elapsedTime = m_state->m_pausableClock - jumpAbility.getStartTime();
		if (elapsedTime >= jumpAbility.getCooldown())
		{
			jumpAbility.setStartTime(0.f);
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
	if (graphics::getKeyState(graphics::SCANCODE_F) && !dashAbility.isRunning())
	{
		dashAbility.setStartTime(m_state-> m_pausableClock);
	}	
	
	if (dashAbility.isRunning())
	{
		float elapsedTime = m_state->m_pausableClock - dashAbility.getStartTime();
		//dash has certain duration, otherwise character teleports to another position and creates bugs like passing through objects
		if (elapsedTime < dashAbility.getDuration())
		{
			m_vx = dashAbility.getSpeed() * m_lookingDirection;
			m_pos_x += delta_time * m_vx;
		}
		if (elapsedTime >= dashAbility.getCooldown())
		{
			dashAbility.setStartTime(0.f) ;
		}
	}
}

void Player::slash(float delta_time)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && !slashAbility.isRunning())
	{	
		damageBox.setActive(true);
		slashAbility.setStartTime(m_state->m_pausableClock);
	}
	if (slashAbility.isRunning())
	{
		float elapsedTime = m_state->m_pausableClock - slashAbility.getStartTime();
		if (elapsedTime < slashAbility.getDuration())
		{
			//extra offset corrected with the player's direction
			float lookingDirOffset = 0.5f * m_lookingDirection;
			//slash damagebox follows player
			damageBox.setPosition(m_pos_x + lookingDirOffset, m_pos_y , 0.8f, 0.8f);
		}
		else if(damageBox.isActive())
		{			
			damageBox.setActive(false);
		}

		if (elapsedTime >= slashAbility.getCooldown())
		{
			slashAbility.setStartTime(0.f);
		}	
	}
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
