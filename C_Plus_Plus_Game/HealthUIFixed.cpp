#include "HealthUIFixed.h"
#include "util.h"
#include "GameState.h"

void HealthUIFixed::init()
{
	m_fontSize = 1;
}

void HealthUIFixed::draw()
{
	// States::Paused is checked because drawText always draws above drawRect
	if (isActive() && m_state->getCurrentState() != States::Paused)
	{
		float calculatedPosX = m_pos_x + m_width / 2;
		float calculatedPosY = m_pos_y + m_height / 2;

		float positionCorrection = (m_width - m_proportionalWidth) / 2;
		graphics::drawRect(calculatedPosX, calculatedPosY, m_width, m_height, m_brush);
		graphics::drawRect(calculatedPosX - positionCorrection, calculatedPosY, m_proportionalWidth, m_height, m_fillBrush);

		float centeringValueX = calcCenteringXForTextSize(m_healthTxt, m_fontSize);
		float centeringValueY = calcCenteringYForTextSize(m_fontSize);
		graphics::drawText(calculatedPosX - centeringValueX, calculatedPosY + centeringValueY, m_fontSize, m_healthTxt, m_valueBrush);

		if (m_state->m_debugging)
		{
			debugDraw(m_pos_x, m_pos_y, m_width, m_height, m_id);
		}
	}
}
