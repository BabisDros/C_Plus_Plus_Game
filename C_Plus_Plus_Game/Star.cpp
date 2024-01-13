#include "Star.h"
#include "GameState.h"
#include "util.h"

void Star::init()
{
	m_height = 0;
	m_width = 0;
	setCustomBrushProperties(&m_brush, 1.0f, 0, m_state->getFullAssetPath("UI\\Star_01.png"));	
}

void Star::draw()
{
	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 4, m_width, m_height, m_brush);
}

void Star::update(float dt)
{	
	float deltaSec = dt / 1000 * growthRate;		
	if (m_height < m_finalSize)
	{
		m_height +=  deltaSec;
		std::cout << m_height << std::endl;
		m_width += deltaSec;
	}
}

bool Star::hasGrown() const
{
	return m_height >= m_finalSize;
}
