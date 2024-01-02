#pragma once
#include <iostream>
#include "GameState.h"
#include "HealthUI.h"
//INTERFACE. Objects that can be destructed should implement this
class IDestructible
{
public:
    

    IDestructible() {};
    virtual ~IDestructible() 
    {
       /* if (healthUi)
            delete healthUi;*/
    }
    

    void setHealths(int health)
    { 
        m_initialHealth = health;
        m_currentHealth = health;
    }
    virtual int getHealth() const { return m_currentHealth; };
    virtual void resetHealth() { m_currentHealth = m_initialHealth; };
    virtual void takeDamage(const int& damage) = 0;
    //{
    //    if (m_currentHealth > 0)
    //    {
    //        float pausableClock = *GameState::getInstance()->getPausableClock();
    //        
    //        if (pausableClock - damageTakenTimestamp  > invisibilityDuration)
    //        {
    //            damageTakenTimestamp = pausableClock;
    //            m_currentHealth -= damage;
    //            //healthUi->refreshHealth(damage, m_initialHealth, m_currentHealth);
    //        }      
    //    }
    //    else
    //    {
    //        destroy();
    //    }
    //};
    virtual bool isAlive() const { return m_currentHealth > 0; };

    //Object is not destroyed but setActive false
    virtual void destroy() = 0;
    virtual void instantiateParticles() = 0;

protected:
    int m_initialHealth = 0;
    int m_currentHealth = 0;
    //duration when the object can take anymore damage. Take damage once per hit
    float invincibilityDuration = 0.5f;
    float damageTakenTimestamp=0.0f;
};