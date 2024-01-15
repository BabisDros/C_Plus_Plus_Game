#include "HealthUIMoving.h"
#include "util.h"
#include "GameState.h"
void HealthUIMoving::init()
{
	setActive(false);
	m_fontSize = 0.2f;
}

void HealthUIMoving::draw()
{
	if (isActive())
	{
		float calculatedPosX = m_pos_x + m_state->m_global_offset_x;
		float calculatedPosY = m_pos_y + m_state->m_global_offset_y - m_offsetY;

		float positionCorrection = (m_width - m_proportionalWidth) / 2;
		graphics::drawRect(calculatedPosX, calculatedPosY, m_width, m_height, m_brush);
		graphics::drawRect(calculatedPosX - positionCorrection, calculatedPosY, m_proportionalWidth, m_height, m_fillBrush);

		float centeringValueX = calcCenteringXForTextSize(m_healthTxt, m_fontSize);
		float centeringValueY = calcCenteringYForTextSize(m_fontSize);
		graphics::drawText(calculatedPosX - centeringValueX, calculatedPosY + centeringValueY, m_fontSize, m_healthTxt, m_valueBrush);

		disableAfterElapsedTime();

		if (m_state->m_debugging)
		{
			debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_width, m_height, m_id);
		}
	}
}

void HealthUIMoving::disableAfterElapsedTime()
{
	if (*m_state->getPausableClock() - m_startTime > m_duration)
	{
		setActive(false);
	}
}
