#include "DamageBox.h"
#include <iostream>

void DamageBox::init()
{
	setActive(false);
}

void DamageBox::draw()
{
	if (isActive())
	{
		if (*m_PlayerDirection <  0)
		{ 
			graphics::setScale(-1.0f, 1.0f);//mirrors image according to player's direction
		}
		graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);
		graphics::resetPose(); //reset mirror for next call	
	}
}

void DamageBox::update(float dt)
{
}
