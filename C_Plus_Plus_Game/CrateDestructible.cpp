#include "CrateDestructible.h"

void CrateDestructible ::init() 
{
	m_brush.texture = *m_texture;
	SETCOLOR(m_brush.fill_color, 1.0, 0.5, 0.0);//orange
}

void CrateDestructible::draw() 
{
	//call base class
	LevelBox::draw();
	healthUi->draw();
}
void CrateDestructible::takeDamage(const int& damage)
{
        if (m_currentHealth > 0)
        {
            float pausableClock = *GameState::getInstance()->getPausableClock();
            if (pausableClock - damageTakenTimestamp > invincibilityDuration)
            {
                damageTakenTimestamp = pausableClock;
                m_currentHealth -= damage;
                healthUi->refresh(damage,m_initialHealth,m_currentHealth);
            }
        }
        else
        {
            destroy();
        }
}

void CrateDestructible::destroy()
{
	setActive(false);
}

void CrateDestructible::instantiateParticles()
{
}
