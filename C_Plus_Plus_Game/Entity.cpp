#include "Entity.h"
#include "Level.h"
#include "GameState.h"
void Entity::checkCollision()
{
	for (auto& block : m_state->getLevel()->getBlocks())
	{
		if (!intersectTypeY(*block))
		{
			float offset;
			if (offset = intersectSideways(*block))	//? Does not go in if 0
			{
				m_pos_x += offset;
				m_vx = 0.0f;
				break;
			}
		}
	}

	for (auto& block : m_state->getLevel()->getBlocks())
	{
		if (intersectTypeY(*block))
		{
			float offset;
			if (offset = intersectY(*block))	//? Does not go in if 0
			{
				m_pos_y += offset;
				if (offset > 0)
				{
		//			m_collidingUp = true;	//Probably not needed cause of cooldown
				}
				else
				{
		//			m_collidingUp = false;
				}
				//? add sound event
				//if (m_vy > 1.0f)
				//	graphics::playSound(m_state->getFullAssetPath("Metal2.wav"), 1.0f);
				m_vy = 0.0f;
				break;
			}
		}
	}
}
