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
	for (int i = 0; i < m_state->getLevel()->getBlocks()->size(); ++i)
	{
		Box* block = &(*m_state->getLevel()->getBlocks())[i]; 

		if (intersect(*block) /*&& typeid(*block) == typeid(Destructible)*/)
		{
			std::cout << "colliding";
		}
	}

}
