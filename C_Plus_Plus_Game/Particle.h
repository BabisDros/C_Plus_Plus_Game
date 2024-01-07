#pragma once
#include "GameObject.h"
#include "sgg/graphics.h"
class Particle
{
	graphics::Brush m_brush;
	float m_pos_x = 0.0f;
	float m_pos_y = 0.0f;
	float m_width = 0.0f;
	float m_height = 0.0f;
	float m_velocity = 0;
	float m_acceleration=0;
	float m_lifetime = 0;
	float m_currentLife = 0;
	float gravity = 0;

	const bool isAlive() const { return m_currentLife > 0; }
	void fade();
};