#pragma once
#include "box.h"
#include <string>
#include "GameObject.h"
#include "util.h"
#include "Gamestate.h"

class LevelBox : public GameObject, public Box
{
protected:
	const std::string* m_texture;
	bool& isDestructible;
public:
	LevelBox(float x, float y, float w, float h, const std::string* texture, bool destructible)
		:Box(x, y, w, h), m_texture(texture), isDestructible(destructible) { init(); }

	void init() override
	{
		m_brush.texture = *m_texture;
		m_brush.outline_opacity = 0.0f;	//? texturing
	}

	void draw() override
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
	const std::string* getTexture()
	{
		return m_texture;
	}

	bool& getIsDestructible() const
	{
		return isDestructible;
	}
};