#pragma once
#include "GameObject.h"
#include "box.h"
#include "GameState.h"
#include <iostream>//debug
class Entity : public Box, public GameObject
{
public:
	template <typename Container>
	void checkCollision(Container myContainer);
	Entity(std::string name) : GameObject(name) {}
	virtual void movement(float dt);

protected:
	bool m_mirrored = false;
	float m_vx = 0.f;
	float m_vy = 0.f;
};

template<typename Container>
inline void Entity::checkCollision(Container myContainer)
{
	
	for (auto& block : myContainer)
	{
		std::cout << typeid(block).name();
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

	for (auto& block : myContainer)
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
