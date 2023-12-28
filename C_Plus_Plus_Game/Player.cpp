#include "Player.h"
#include <sgg/graphics.h>
#include "GameState.h"
#include "util.h"
#include <iostream>

void Player::init()
{
	m_pos_x = m_state->getCanvasWidth() / 4.0f; //
	m_pos_y = m_state->getCanvasHeight() - 1.f;	//?? make initial var accesible

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("temp_player2.png");	
	
}

void Player::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image

	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brush_player);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw();
	}

}

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	m_state->enable(m_dev_fly, m_dev_fly_held, graphics::getKeyState(graphics::SCANCODE_MINUS));
	if (m_dev_fly) fly(delta_time);
	else movePlayer(delta_time);	//! player can currently hover below blocks if holding W, needs fix

	if (m_pos_y > m_state->getCanvasHeight() + 2) //? is in void
	{
		m_pos_x = m_state->getCanvasWidth() / 2.0f; //? centered
		m_pos_y = m_state->getCanvasHeight() / 2.0f; //! should be made to get to starting position
	}

	cameraOffsetX(0.4f, 0.6f);		//! Preferably make it a single function {with array, enum?}, only if it isn't complex
	cameraOffsetY(0.3f, 0.7f);

	dash(delta_time);
}

void Player::dash(float delta_time)
{
	if (graphics::getKeyState(graphics::SCANCODE_F) && !m_myTimer.isRunning())
	{
		m_myTimer = Timer(m_dash_cooldown);
		m_myTimer.start();
		m_dashStartTime = graphics::getGlobalTime() / 1000.f;
	}
	//allow dash movement to complete in certain time. Otherwise character teleports to another position and creates bugs like passing through objects
	if (m_myTimer.isRunning())
	{
		if (graphics::getGlobalTime() / 1000.f - m_dashStartTime < m_dashDuration)
		{
			if (m_mirrored)
			{
				m_vx = -m_dashSpeed;
			}
			else
			{
				m_vx = m_dashSpeed;
			}
			m_pos_x += delta_time * m_vx;

		}
		float timer = m_myTimer.operator float();
		// Check if the cooldown duration has passed
		if (timer >= 1.0f) //
		{
			m_myTimer.stop();
		}
	}
}
void Player::movePlayer(float delta_time)
{

	float move = 0;

	if (graphics::getKeyState(graphics::SCANCODE_A))	//? movement
	{
		move = -1.0f;
		m_mirrored = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		move = 1.0f;
		m_mirrored = false;
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

float Player::jump() const
{
	if (m_vy == 0.0f && !m_collidingUp)	//! need better if for jump, [check for top, bottom collision?]
	{
		return (graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f) * 0.02f;//? not delta_time! Burst [Papaioannou comment]
	}

	else return 0;
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

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;

	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, debug_brush);
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
