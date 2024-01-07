#pragma once
#include "DamageBox.h"
#include <iostream>
#include "Util.h"
#include "GameState.h"
#include "Level.h"
#include "Player.h"

void DamageBox::setPosition(float x, float y, float width, float height)
{
	m_pos_x = x;
	m_pos_y = y;
	m_width = width;
	m_height = height;
}

void DamageBox::init()
{
	setActive(false);
}

void DamageBox::draw()
{
	if (isActive())
	{
		if (m_parentDirection < 0 && m_canBeMirrored)
		{ 
			graphics::setScale(-1.0f, 1.0f);//mirrors image according to parent object direction
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
		if (m_parentIsPlayer)
		{
			checkForCollisions(m_state->getLevel()->getDestructibleObjects());
		}
		else
		{
			CollisionObject* player= dynamic_cast<CollisionObject*>(m_state->getPlayer());
			checkForCollisions(player);
		}
	}
}
void DamageBox::setParentIsPlayer(bool isPlayer)
{
	m_parentIsPlayer = isPlayer;
}
void DamageBox::setBrush(const graphics::Brush& brush)
{
	m_brush = brush;
}

void DamageBox::checkForCollisions(const std::list<CollisionObject*> containerToScan)
{
	for (auto& gameobj: containerToScan)
	{
		//check if the objects of the container are destructible
		IDestructible* destructiblePtr = dynamic_cast<IDestructible*>(gameobj);
		
		/*all objects will be setActive(false) when health is 0. 
		TODO: if performance improvement needed : could delete them, and shorten the list*/
		if (intersect(*gameobj) && destructiblePtr)
		{		
			destructiblePtr->takeDamage(m_damageToInflict);
		}
	}
}

void DamageBox::setParentDirection(const int& direction)
{
	m_parentDirection = direction;
}

void DamageBox::checkForCollisions(CollisionObject* player)
{
	//check if the objects of the container are destructible
	IDestructible* destructiblePtr = dynamic_cast<IDestructible*>(player);

	/*all objects will be setActive(false) when health is 0.
	TODO: if performance improvement needed : could delete them, and shorten the list*/
	if (intersect(*player) && destructiblePtr)
	{
		destructiblePtr->takeDamage(m_damageToInflict);
	}
	
}

void DamageBox::setDamageToInflict(int damage)
{
	m_damageToInflict = damage;
}




