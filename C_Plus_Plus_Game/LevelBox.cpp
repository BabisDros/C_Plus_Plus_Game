#include "LevelBox.h"

void LevelBox::init()
{
	m_brush.texture = *m_texture;
	m_brush.outline_opacity = 0.0f;	//? texturing
}

void LevelBox::draw()
{
	if (isActive())
	{
		float x = m_pos_x + m_state->m_global_offset_x;
		float y = m_pos_y + m_state->m_global_offset_y;
		m_brush.texture = m_state->getFullAssetPath(*m_texture);
		graphics::drawRect(x, y, m_width, m_height, m_brush);

		if (m_state->m_debugging) debugDraw(x, y, m_width, m_height, getId());
	}
}

const std::string* LevelBox::getTexture()
{
	return m_texture;
}

bool& LevelBox::getIsDestructible() const
{
	return m_isDestructible;
}
