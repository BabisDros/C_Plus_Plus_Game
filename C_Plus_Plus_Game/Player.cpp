#include "Player.h"
#include <sgg/graphics.h>
#include "GameState.h"

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
	/* background is static */
	//graphics::drawRect(m_pos_x, m_pos_y, 1.0f, 1.0f, m_brush_player);

	/* background is moving */
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
}

void Player::update(float dt)
{
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
	
	// Used if we want to have player be on the center always
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

}
