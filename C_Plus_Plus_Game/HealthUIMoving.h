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
		m_fontSize = 0.2f;
	}

	void draw() override
	{
		if (isActive())
		{		
			float calculatedPosX = m_pos_x + m_state->m_global_offset_x;
			float calculatedPosY = m_pos_y + m_state->m_global_offset_y - m_offsetY;

			float positionCorrection = (m_width - m_proportionalWidth) / 2;
			graphics::drawRect(calculatedPosX, calculatedPosY, m_width, m_height, m_brush);
			graphics::drawRect(calculatedPosX - positionCorrection, calculatedPosY, m_proportionalWidth, m_height, m_fillBrush);
			
			float centeringValueX = calcCentering×ForTextSize(m_healthTxt.size(), m_fontSize);
			float centeringValueY = calcCenteringYForTextSize(m_fontSize);
			graphics::drawText(calculatedPosX - centeringValueX, calculatedPosY + centeringValueY, m_fontSize, m_healthTxt, m_valueBrush);

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
