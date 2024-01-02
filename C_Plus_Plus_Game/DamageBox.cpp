#include "DamageBox.h"
#include <iostream>
#include "IDestructible.h"
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
	}
	if (m_state->m_debugging)
	{		
		debugDraw(m_pos_x+ m_state->m_global_offset_x, m_pos_y+ m_state->m_global_offset_y, m_width, m_height);
	}
	
}

void DamageBox::update(float dt)
{	
	if (isActive())
	{
		for (int i = 0; i < m_state->getLevel()->getBlocks().size(); ++i)
		{
			LevelBox* block = (m_state->getLevel()->getBlocks())[i];
			
			if (intersect(*block) && block->getIsDestructible()/*&& typeid(*block) == typeid(IDestructible)*/)
			{

				std::cout << "Damage box pos "<< m_pos_x << " block pos " << block->getId()<< std::endl;
				
			}
		}
	}
}
