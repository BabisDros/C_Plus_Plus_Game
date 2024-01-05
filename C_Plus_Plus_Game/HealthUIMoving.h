#pragma once
#include "HealthUI.h"
//add a ui of this type if you want to appear above object and follow it
class HealthUIMoving :public HealthUI
{
	//adjust offset to appear above gameobject. 
	float m_offsetY = 1.f;
public:
	HealthUIMoving() :HealthUI() { init(); }
	HealthUIMoving(float x, float y, float width, float height) :HealthUI(x, y, width, height) { init(); }

	void init() override
	{
		setActive(false);
		HealthUI::init();
	}

	void draw() override
	{
		if (isActive())
		{
			float positionCorrection = (m_width - m_proportionalWidth) / 2;
			graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_width, m_height, m_brush);
			graphics::drawRect(m_pos_x + m_state->m_global_offset_x - positionCorrection, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_proportionalWidth, m_height, m_fill);

			disableAfterElapsedTime();

			if (m_state->m_debugging)
			{
				debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_width, m_height, m_id);
			}
		}
	}

	void disableAfterElapsedTime()
	{
		if (*m_state->getPausableClock() - m_startTime > m_duration)
		{
			setActive(false);
		}
	}
};
