#include "DamageBox.h"
#include <iostream>
#include "Destructible.h"
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
		graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);
		graphics::resetPose(); //reset mirror for next call	
	}
	if (m_state->m_debugging)
	{
		for (int i = 0; i < m_state->getLevel()->getBlocks()->size(); ++i)
		{
			LevelBox* block = &(*m_state->getLevel()->getBlocks())[i];
			debugDraw(block->m_pos_x /*+ m_state->m_global_offset_x*/, block->m_pos_y /*+ m_state->m_global_offset_y*/, block->m_width, block->m_height,std::to_string( block->getId()));

		}
	}
	
}

void DamageBox::update(float dt)
{
	
	if (isActive())
	{
		for (int i = 0; i < m_state->getLevel()->getBlocks()->size(); ++i)
		{
			LevelBox* block = &(*m_state->getLevel()->getBlocks())[i];
			
			if (intersect(*block) && block->getIsDestructible()/*&& typeid(*block) == typeid(Destructible)*/)
			{
				std::cout << "Damage box pos "<< m_pos_x << "block pos " << block->getName()<< std::endl;
				
			}
		}
	}

	

}
