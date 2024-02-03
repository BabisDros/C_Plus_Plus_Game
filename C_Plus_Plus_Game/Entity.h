#pragma once
#include "CollisionObject.h"
#include "GameState.h"
#include "CrateDestructible.h"
#include <iostream>//debug

class Entity :public CollisionObject
{
public:
	template <typename Container>
	void checkCollision(Container myContainer);
	Entity(){}
	Entity(std::string name) : CollisionObject(name) {}
	Entity(std::string name, float pos_x, float pos_y, float width, float height) : CollisionObject(name, pos_x, pos_y, width, height) {}
	virtual void movement(float dt)=0;

	const std::string* m_texture;
protected:
	bool m_mirrored = false; // used for draw
	float m_vx = 0.f;
	float m_vy = 0.f;
	int m_lookingDirection = 1; //looking left value: -1 | looking right value: 1
	bool m_collidingDown;
	bool m_collidingUp;
	bool m_collisingLR;
};

template<typename Container>
inline void Entity::checkCollision(Container myContainer)
{

	for (auto& block : myContainer)
	{
		if (typeid(*block).name() == typeid(*this).name()) continue;	//No collision between enemies, but allowed between destructibles
		if (block->isActive())
		{
			if (!intersectTypeY(*block))
			{
				float offset;
				if (offset = intersectSideways(*block))	//? Does not go in if 0
				{
					m_pos_x += offset;
					m_vx = 0.0f;
					m_collisingLR = true;
					break;
				}
			}
		}		
	}
	
	for (auto& block : myContainer)
	{
		if (typeid(*block).name() == typeid(*this).name()) continue;	//No collision between enemies, but allowed between destructibles
		if (block->isActive())
		{
			if (intersectTypeY(*block) && intersect(*block))
			{
				float offset;
				if (offset = intersectY(*block))	//? Does not go in if 0
				{
					m_pos_y += offset;
					if (offset < 0)
					{
						m_collidingDown = true;
					}
					else
					{
						m_collidingUp = true;
					}
					m_vy = 0.0f;
					break;
				}
			}
			
		}
	}
}
