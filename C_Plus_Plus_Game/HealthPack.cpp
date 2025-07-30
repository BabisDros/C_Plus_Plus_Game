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

void HealthPack::update(const float& dt)
{
	if (isActive())
	{
		CollisionObject* player = dynamic_cast<CollisionObject*>(m_state->getPlayer());
		IDestructible* destructiblePtr = dynamic_cast<IDestructible*>(player);
		if (intersect(*player))
		{
			destructiblePtr->takeDamage(m_heal_amount);
			graphics::playSound("music\\580814_silverillusionist_healing-3-soothing-rinse.wav", 0.1f);
			destroy();
		}
	}
}

void HealthPack::destroy()
{
	setActive(false);
}

