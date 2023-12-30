#include "DamageBox.h"
#include <iostream>
#include "Destructible.h"
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
}

void DamageBox::update(float dt)
{
	for (auto& block : m_state->getLevel()->getBlocks())
	{
		if (intersect(block) && block->/*typeid(block)== typeid(Destructible)*/)
		{
			std::cout << "colliding";
		}
	}
}
