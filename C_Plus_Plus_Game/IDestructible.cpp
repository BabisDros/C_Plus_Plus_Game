#include "IDestructible.h"
#include "GameState.h"
#include "CallbackManager.h"
void IDestructible::setInitialHealthValues(const int& health)
{
    m_initialHealth = health;
    m_currentHealth = health;
}

void IDestructible::setHealth(const int& health)
{
    m_currentHealth = health;
    CallbackManager::getInstance()->m_playerHealthChanged.trigger(IDestructible::m_initialHealth, IDestructible::m_currentHealth);
}

int IDestructible::getHealth() const
{
    return m_currentHealth;
}
void IDestructible::resetHealth()
{
    m_currentHealth = m_initialHealth;
}

void IDestructible::takeDamage(const int& damage)
{
    if (m_currentHealth > 0)
    {
        float pausableClock = *GameState::getInstance()->getPausableClock();
        if (pausableClock - m_damageTakenTimestamp > m_invincibilityDuration)
        {
            m_damageTakenTimestamp = pausableClock;
            if (m_currentHealth - damage < m_initialHealth) m_currentHealth -= damage;
            else resetHealth();
            if (m_healthUi)
                m_healthUi->updateUIOnDamage(m_initialHealth, m_currentHealth);
            if (m_currentHealth <= 0) destroy();
        }
    }

    else  if (isAlive())
    {
        destroy();
    }
}

bool IDestructible::isAlive() const
{
    return m_currentHealth > 0;
}
