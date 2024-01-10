#include "HealthPack.h"
#include "Player.h"

void HealthPack::init()
{
	setActive(true);
}

void HealthPack::draw()
{
	LevelBox::draw();
}

void HealthPack::update(float dt)
{
	if (isActive())
	{
		CollisionObject* player = dynamic_cast<CollisionObject*>(m_state->getPlayer());
		IDestructible* destructiblePtr = dynamic_cast<IDestructible*>(player);
		if (intersect(*player))
		{
			destructiblePtr->takeDamage(m_heal_amount);
			graphics::playSound("music\\heal.wav", 0.30f);
			destroy();
		}
	}
}

void HealthPack::destroy()
{
	setActive(false);
}

