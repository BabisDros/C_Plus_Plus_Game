#include "HealthUi.h"
#include "Util.h"
#include "GameState.h"
void HealthUI::init()
{
	SETCOLOR(m_brush.fill_color, 1.0, 0.0, 0.0);//background red
	SETCOLOR(m_fillBrush.fill_color, 0.0, 1.0, 0.0);//green
	SETCOLOR(m_valueBrush.fill_color, 1.0f, 1.0f, 1.0f);
	setCustomBrushProperties(&m_brush, 0.5f, 0.5f);
	setCustomBrushProperties(&m_fillBrush, 0.5f, 0.5f);
	setCustomBrushProperties(&m_valueBrush, 0.8f, 0.8f);
}

void HealthUI::setPosition(float x, float y)
{
	m_pos_x = x;
	m_pos_y = y;
}

void HealthUI::setSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

void HealthUI::updateUIOnDamage(const int& initialHealth, const int& currentHealth)
{
	m_startTime = *m_state->getPausableClock();
	//reduces fill size once, so that the red background appears
	m_proportionalWidth = currentHealth * m_width / initialHealth;
	m_healthTxt = std::to_string(currentHealth);

	setActive(true);
}

float HealthUI::getHeight() const
{
	return m_height;
}
