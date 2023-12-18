#include "Player.h"
#include <sgg/graphics.h>
#include "GameState.h"
#include "util.h"

void Player::init()
{
	m_pos_x = 3.0f;
	m_pos_y = 3.0f;

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

	m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
	m_vx = std::max(-m_max_velocity, m_vx);
	//m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx)); //slow down

	if (!graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) //insta stop, doesn't happen when both are held
	{
		m_vx = 0;
	}

	m_pos_x += delta_time * m_vx;
	/* need to to up and down*/
	move = 0;
	if (graphics::getKeyState(graphics::SCANCODE_W))
	{
		move -= 1.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S))
	{
		move += 1.0f;
	}


	m_vy = std::min(m_max_velocity, m_vy + delta_time * move * m_accel_vertical);
	m_vy = std::max(-m_max_velocity, m_vy);
	m_pos_y += delta_time * m_vy;
	m_vy -= 0.2f * m_vy / (0.1f + fabs(m_vy)); //slow down
}

void Player::debugDraw()
{
	// needs implementation
	m_brush_player.outline_opacity = 0.3f;
	SETCOLOR(m_brush_player.fill_color, 0.1f, 0.f, 0.1f);
}