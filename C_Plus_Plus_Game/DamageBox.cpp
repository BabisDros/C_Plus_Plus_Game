#pragma once
#include "DamageBox.h"
#include "Util.h"
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
		if ((m_parentDirection < 0 && m_canActivelyBeMirrored) || (!m_canActivelyBeMirrored && m_draw_direction < 0))
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


void DamageBox::update(const float& dt)
{	
	if (isActive())
	{
		checkForCollisions(m_state->getLevel()->getDestructibleObjects());
		if (!m_parentIsPlayer)
		{
			CollisionObject* player= dynamic_cast<CollisionObject*>(m_state->getPlayer());
			checkForCollisions(player); // damage player
			checkForCollisions(m_state->getLevel()->getBlocks()); // stop projectiles on walls
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
	for (auto* gameobj: containerToScan)
	{		
		//all objects will be setActive(false) when health is 0. 
		
		if (intersect(*gameobj) && gameobj->isActive()) // contact was found
		{		
			if (m_parentIsPlayer)
			{
				//check if the objects of the container are destructible
				IDestructible* destructiblePtr = dynamic_cast<IDestructible*>(gameobj);
				if (destructiblePtr && gameobj->m_can_die) destructiblePtr->takeDamage(m_damageToInflict);
			}
			else
			{
				CrateDestructible* cratePtr = dynamic_cast<CrateDestructible*>(gameobj);
				if (cratePtr) // it came in contact with crate and not enemy
				{
					m_canMove = false;
					break;
				}
			}
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

	//all objects will be setActive(false) when health is 0.
	
	if (intersect(*player) && destructiblePtr)
	{
		pushPlayer();
		destructiblePtr->takeDamage(m_damageToInflict);
		if (m_diesOnTouch) setActive(false);
	}	
}

void DamageBox::checkForCollisions(const std::vector<LevelBox*> wall)
{
	for (LevelBox* itr : wall)
	{
		if (intersect(*itr))
		{
			m_canMove = false;
			break;
		}
	}
}

void DamageBox::setDamageToInflict(int damage)
{
	m_damageToInflict = damage;
}

void DamageBox::pushPlayer()
{
	m_state->getPlayer()->setPushed(m_pos_x, m_pos_y);
}