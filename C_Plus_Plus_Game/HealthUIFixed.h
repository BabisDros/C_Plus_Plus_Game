#pragma once
#include "HealthUI.h"

//add a ui of this type so that it appears fixed on position of your choice
class HealthUIFixed :public HealthUI
{
public:
	HealthUIFixed() :HealthUI() {}
	HealthUIFixed(float x, float y, float width, float height) :HealthUI(x, y, width, height ) {}

	void draw() override
	{
		if (isActive())
		{
			float positionCorrection = (m_width - m_proportionalWidth) / 2;
			graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);
			graphics::drawRect(m_pos_x - positionCorrection, m_pos_y, m_proportionalWidth, m_height, m_fill);

			if (m_state->m_debugging)
			{
				debugDraw(m_pos_x, m_pos_y, m_width, m_height, m_id);
			}
		}
	}
};