#pragma once
#include "CrateDestructible.h"
#include "util.h"
#include "GameEvents.h"
#include "Level.h"
#include "HealthPack.h"

void CrateDestructible ::init() 
{
	m_brush.texture = *m_texture;
	SETCOLOR(m_brush.fill_color, 1.0, 0.5, 0.0);//orange
	if (m_loot == Nothing)
	{
		// nothing
	}
	else if (m_loot == Health_pack)
	{
		//  HP
		m_has_health = true;
	}
	else if (m_loot == Extra_loot)
	{
		// Triple loot
		m_extra_points = true;
		m_points *= 3;
	}
}

void CrateDestructible::draw() 
{
	//call base class
	LevelBox::draw();
	m_healthUi->draw();
}

void CrateDestructible::destroy()
{
	setActive(false);
	if (m_has_health) 
	{
		m_state->getLevel()->getDestructibleObjectsPtr()->push_back(new HealthPack(m_pos_x, m_pos_y + m_width/4, m_width/2, m_height/2, &m_health_texture, false)); // should happen once
		m_state->getLevel()->getDestructibleObjects().back()->init();
	}
	if (m_loot == Extra_loot) graphics::playSound("music\\extra_loot.wav", 0.03f);
	GameEvents::getInstance()->m_pointsChanged.trigger(m_points,false);
}
