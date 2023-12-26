#include "Player.h"
#include <sgg/graphics.h>
#include "GameState.h"
#include "util.h"
#include <iostream>

void Player::init()
{
	m_pos_x = m_state->getCanvasWidth() / 2.0f; // centered
	m_pos_y = m_state->getCanvasHeight() / 2.0f;	//!! make initial var accesible

	// Used if we want to have player be on the center always
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("temp_player2.png");
	
}

void Player::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image

	/* background is moving */
	if (m_state->m_camera_follow_player)
	{
		// Below previous state
		//graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
		//======================================================
		if (!m_state->m_camera_follow_player_x)
		{
			graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_state->getCanvasHeight() * m_camera_multiplier_y, 1.0f, 1.0f, m_brush_player);
		}
		else if (!m_state->m_camera_follow_player_y)
		{
			graphics::drawRect(m_state->getCanvasWidth() * m_camera_multiplier_x, m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brush_player);
		}
		else
		{ 
			graphics::drawRect(m_state->getCanvasWidth() * m_camera_multiplier_x, m_state->getCanvasHeight() * m_camera_multiplier_y, 1.0f, 1.0f, m_brush_player);	//!! 0.25 has to be var 
		}
	}
	else /* background is static */
	{
		graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brush_player);
	}

	graphics::resetPose(); //reset mirror for next call

	//new for debug, need to be implemented
	if (m_state->m_debugging)
	{
		debugDraw();
	}
}

void Player::update(float dt)
{
	m_state->enable(m_dev_fly, m_dev_fly_held, graphics::getKeyState(graphics::SCANCODE_MINUS));
	if (m_dev_fly) fly(dt);
	else movePlayer(dt);

	if (m_pos_y > m_state->getCanvasHeight() + 2) //? is in void
	{
		m_pos_x = m_state->getCanvasWidth() / 2.0f; //? centered, aka starting position
		m_pos_y = m_state->getCanvasHeight() / 2.0f;
		m_state->m_global_offset_x = 0;
		m_state->m_global_offset_x = 0;
		m_state->m_global_offset_y = 0;
	}

	cameraOffsetX(0.25f, 0.75f);		//! Preferably make it a single function {with array, enum?}
	cameraOffsetY(0.25f, 0.75f);
}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;
	float move = 0.f;
	if (graphics::getKeyState(graphics::SCANCODE_A))	//? movement
	{
		move -= 1.0f;
		m_mirrored = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		move += 1.0f;
		m_mirrored = false;
	}

	if (!graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D) ||
		graphics::getKeyState(graphics::SCANCODE_D) && graphics::getKeyState(graphics::SCANCODE_A)) //? insta stop
	{
		m_vx = 0.f;
	}
	else
	{
		m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
		m_vx = std::max(-m_max_velocity, m_vx);
		//m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx)); //? slow down
	}

	m_pos_x += delta_time * m_vx;

	move = 0.f;

	if (m_vy == 0.0f)
	{ 
		m_vy -= (graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f) * 0.02f;//? not delta_time!! Burst [Papaioannou comment]
	}
	m_vy = std::min(m_max_velocity, m_vy + delta_time * move * m_accel_vertical);
	m_vy = std::max(-m_max_velocity, m_vy);
	
	//m_vy -= 0.2f * m_vy / (0.1f + fabs(m_vy)); //? slow down
	m_vy += delta_time * m_gravity;
	m_pos_y += delta_time * m_vy;
}

void Player::fly(float dt)
{
	float delta_time = dt / 1000.0f;
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
	if (m_state->m_camera_follow_player)
	{ 
		if (!m_state->m_camera_follow_player_x)
		{
			graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_state->getCanvasHeight() * m_camera_multiplier_y, 1.0f, 1.0f, debug_brush);
		}
		else if (!m_state->m_camera_follow_player_y)
		{
			graphics::drawRect(m_state->getCanvasWidth() * m_camera_multiplier_x, m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, debug_brush);
		}
		else
		{
			graphics::drawRect(m_state->getCanvasWidth() * m_camera_multiplier_x, m_state->getCanvasHeight() * m_camera_multiplier_y, 1.0f, 1.0f, debug_brush);	//!! 0.25 has to be var 
		}
	}
	else
	{ 
		graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, debug_brush);
	}
}


void Player::cameraOffsetX(float multiplier1, float multiplier2)
{
	if (m_pos_x + m_state->m_global_offset_x <= m_state->getCanvasWidth() * multiplier1) //?? based on starting position
	{
		m_state->m_camera_follow_player = true;
		m_state->m_global_offset_x = m_state->getCanvasWidth() * multiplier1 - m_pos_x;
			
		m_camera_multiplier_x = multiplier1;
		m_state->m_camera_follow_player_x = true;
	}
	else if (m_pos_x + m_state->m_global_offset_x >= m_state->getCanvasWidth() * multiplier2) //?? based on starting position
	{
		m_state->m_camera_follow_player = true;
		m_state->m_global_offset_x = m_state->getCanvasWidth() * multiplier2 - m_pos_x;
			
		m_camera_multiplier_x = multiplier2;
		m_state->m_camera_follow_player_x = true;
	}
	else
	{
		m_state->m_camera_follow_player = false;
		m_state->m_camera_follow_player_x = false;	
	}
}

void Player::cameraOffsetY(float multiplier1, float multiplier2)
{
	if (m_pos_y + m_state->m_global_offset_y <= m_state->getCanvasHeight() * multiplier1) //?? based on starting position
	{
		m_state->m_camera_follow_player = true;
		m_state->m_global_offset_y = m_state->getCanvasHeight() * multiplier1 - m_pos_y; 

		m_camera_multiplier_y = multiplier1;
		m_state->m_camera_follow_player_y = true;
	}
	else if (m_pos_y + m_state->m_global_offset_y >= m_state->getCanvasHeight() * multiplier2) //?? based on starting position
	{
		m_state->m_camera_follow_player = true;
		m_state->m_global_offset_y = m_state->getCanvasHeight() * multiplier2 - m_pos_y; 
			
		m_camera_multiplier_y = multiplier2;
		m_state->m_camera_follow_player_y = true;
	}
	else
	{
		m_state->m_camera_follow_player = false;
		m_state->m_camera_follow_player_y = false;
	}
}
