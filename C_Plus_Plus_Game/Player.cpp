#include "Player.h"
#include <sgg/graphics.h>
#include "GameState.h"
#include "util.h"

void Player::init()
{
	m_pos_x = m_state->getCanvasWidth() / 2.0f; // centered
	m_pos_y = m_state->getCanvasHeight() / 2.0f;

	// Used if we want to have player be on the center always
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("temp_player2.png");
	
}

void Player::draw()
{
	//graphics::setScale(-1.0f, 1.0f); mirrors image

	/* background is static */
	//graphics::drawRect(m_pos_x, m_pos_y, 1.0f, 1.0f, m_brush_player);

	/* background is moving */
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);

	//graphics::resetPose(); reset mirror for next call

	//new for debug, need to be implemented
	if (m_state->m_debugging)
	{
		debugDraw();
	}
}

void Player::update(float dt)
{
	/* old movement
		float delta_time = dt / 1000.0f;


		const float velocity = 10.0f;
			if (graphics::getKeyState(graphics::SCANCODE_A))	//movement
		{
			m_pos_x -= velocity * delta_time;
		}
		if (graphics::getKeyState(graphics::SCANCODE_D))
		{
			m_pos_x += velocity * delta_time;
		}
		if (graphics::getKeyState(graphics::SCANCODE_W))
		{
			m_pos_y -= velocity * delta_time;
		}
		if (graphics::getKeyState(graphics::SCANCODE_S))
		{
			m_pos_y += velocity * delta_time;
		}
		*/

	movePlayer(dt);
	if (m_pos_y > m_state->getCanvasHeight() + 2) // is in void
	{
		m_pos_x = m_state->getCanvasWidth() / 2.0f; // centered
		m_pos_y = m_state->getCanvasHeight() / 2.0f;
	}

	// Used if we want to have player be on the center always
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;
	float move = 0.f;
	if (graphics::getKeyState(graphics::SCANCODE_A))	//movement
	{
		move -= 1.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		move += 1.0f;
	}

	if (!graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D) ||
		graphics::getKeyState(graphics::SCANCODE_D) && graphics::getKeyState(graphics::SCANCODE_A)) //insta stop
	{
		m_vx = 0.f;
	}
	else
	{
		m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
		m_vx = std::max(-m_max_velocity, m_vx);
		//m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx)); //slow down
	}

	m_pos_x += delta_time * m_vx;

	move = 0.f;

	if (m_vy == 0.0f)
	{ 
		m_vy -= (graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f) * 0.02f;// not delta_time!! Burst 
	}
	m_vy = std::min(m_max_velocity, m_vy + delta_time * move * m_accel_vertical);
	m_vy = std::max(-m_max_velocity, m_vy);
	
	//m_vy -= 0.2f * m_vy / (0.1f + fabs(m_vy)); //slow down
	m_vy += delta_time * m_gravity;
	m_pos_y += delta_time * m_vy;
}

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);

}