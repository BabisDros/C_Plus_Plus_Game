#pragma once
#include "DamageBox.h"
#include <iostream>
#include "Util.h"

void DamageBox::init()
{
	setActive(false);
}

void DamageBox::draw()
{
	if (isActive())
	{
		if (*m_parentDirection <  0)
		{ 
			graphics::setScale(-1.0f, 1.0f);//mirrors image according to player's direction
		}
		graphics::drawRect(m_pos_x+ m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_brush);
		graphics::resetPose(); //reset mirror for next call	
		
		if (m_state->m_debugging)
		{
			debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_id);
		}
	}
}

void DamageBox::update(float dt)
{	
	if (isActive())
	{
		checkForCollisions();		
	}
}

void DamageBox::checkForCollisions()
{
	for (auto& gameobj:  m_state->getLevel()->getDynamicObjects())
	{
		//all gameobjects in DynamicObjects considered destructible
		IDestructible* destructiblePtr = dynamic_cast<IDestructible*>(gameobj);
		
		//all objects will be setActive(false) when health is 0. TODO if performance improvement needed: could delete them, and shorten the list
		if (intersect(*dynamic_cast<Box*>(gameobj)) && destructiblePtr)
		{		
			destructiblePtr->takeDamage(10);
		}
	}
}




