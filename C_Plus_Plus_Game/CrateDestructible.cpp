#pragma once
#include "CrateDestructible.h"

void CrateDestructible ::init() 
{
	m_brush.texture = *m_texture;
	SETCOLOR(m_brush.fill_color, 1.0, 0.5, 0.0);//orange
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
}

void CrateDestructible::instantiateParticles()
{
}
