#pragma once
#include "IDestructible.h"
#include "LevelBox.h"
class CrateDestructible:public LevelBox, public IDestructible
{
public:
	CrateDestructible(int initialHealth, float x, float y, float w, float h, const std::string* texture, bool destructible) 
		: LevelBox(x, y, w, h, texture, destructible) 
	{
		init();
		setHealths(initialHealth);
	}

	void init() override
	{
		m_brush.texture =* m_texture;		
		SETCOLOR(m_brush.fill_color, 1.0, 0.5, 0.0);
	}

	void destroy() override;
	void instantiateParticles() override;
};